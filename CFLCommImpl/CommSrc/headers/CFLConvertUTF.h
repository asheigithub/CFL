#ifndef CFL_CONVERTUTF_H
#define CFL_CONVERTUTF_H

#include <memory>
#include "GBKTable.h"

/// <summary>  
/// 转换UTF8编码到UCS4编码  
/// </summary>  
/// <param name="UTF8">UTF8编码的字节数组</param>   
/// <param name="UCS4">要输出的UCS4编码</param>  
/// <returns>字节数组中参与编码转换的字节长度</returns>   
static size_t UTF8ToUCS4(const unsigned char* UTF8, unsigned int& UCS4)
{
	auto b = UTF8;

	
	if (*b < 0x80)
	{
		UCS4 = *b;
		return 1;
	}
	if (*b < 0xC0 || *b > 0xFD)
	{	// 非法UTF8
		UCS4 = 0xFFFFFFFF;
		return 0;
	}
	size_t Len;
	if (*b < 0xE0)
	{
		UCS4 = (*b & 0x1F);
		Len = 2;
	}
	else if (*b < 0xF0)
	{
		UCS4 = (*b & 0x0F);
		Len = 3;
	}
	else if (*b < 0xF8)
	{
		UCS4 = (*b & 7);
		Len = 4;
	}
	else if (*b < 0xFC)
	{
		UCS4 = (*b & 3);
		Len = 5;
	}
	else
	{
		UCS4 = (*b & 1);
		Len = 6;
	}
	for (size_t i = 1; i < Len; i++)
	{
		
		if (*(b + i)=='\0')
		{   // 非法UTF8
			UCS4 = 0xFFFFFFFF;
			return 0;
		}

		auto bb = *(b + i);
		if (bb < 0x80 || bb > 0xBF)
		{	// 非法UTF8
			UCS4 = 0xFFFFFFFF;
			return 0;
		}

		UCS4 = ((UCS4 << 6) + (bb & 0x3F));


	}
		
	return Len;
}



static size_t UTF8ToUCS4(const unsigned char* UTF8,unsigned int* UCS4Target)
{
	auto pos = UTF8;
	auto size = 0;
	while (*pos !='\0')
	{
		unsigned int ucs4 = 0;
		auto add= UTF8ToUCS4(pos, ucs4);

		if (add > 0)
		{
			
			pos += add;

			*(UCS4Target + size) = ucs4;
			size++;

		}
		else
		{
			pos++;
		}
		
	}
	return size;

}

/// <summary>  
/// 转换UCS4编码到UTF8编码  
/// </summary>   
static size_t UCS4TOUTF8(const unsigned int ucs4code, unsigned char* utf8)
{


	unsigned int CodeUp[6] = {
		                0x80,           // U+00000000 ～ U+0000007F  
		                0x800,          // U+00000080 ～ U+000007FF  
		                0x10000,        // U+00000800 ～ U+0000FFFF  
		                0x200000,       // U+00010000 ～ U+001FFFFF  
		                0x4000000,      // U+00200000 ～ U+03FFFFFF  
		                0x80000000      // U+04000000 ～ U+7FFFFFFF  
		 };
	
		 // 根据UCS4编码范围确定对应的UTF8编码字节数  
		int Len = -1;
		for (size_t i = 0; i <6; i++)
		{
		    if (ucs4code < CodeUp[i])
			{
			    Len = i + 1;
			    break;
			}
		}
	
		if (Len == -1) return 0;   // 无效的UCS4编码  
	
		// 转换为UTF8编码  
		//Byte[] UTF8 = new Byte[Len];

		unsigned char Prefix[6] = { 0, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

		unsigned int ucs = ucs4code;

		for (size_t i = Len - 1; i > 0; i--)
		{
			utf8[i] = static_cast<unsigned char>((ucs & 0x3F) | 0x80);
			ucs >>= 6;
		}
	
		utf8[0] = static_cast<unsigned char>(ucs | Prefix[Len - 1]);
	
		return  static_cast<size_t>( Len);

}


static size_t UCS4TOUTF8(const unsigned int* ucs4, unsigned char* UTF8,size_t ucs4len)
{
	auto pos = UTF8;
	auto size = 0;
	size_t idx = 0;

	while (idx < ucs4len)
	{
		
		unsigned char utf8temp[6];

		auto add = UCS4TOUTF8( ucs4[idx], utf8temp);

		if (add > 0)
		{
			memcpy(pos, utf8temp, add);

			pos += add;
			size += add;
		}
		

		idx++;
	}
	return size;

}


static size_t GBKTOUTF8(const char* gbk, char* UTF8,size_t gbklen)
{
	auto len = gbklen;

	char* temp = UTF8;
	
	auto  ctemp = temp;

	auto cc = reinterpret_cast<const unsigned char*>(gbk);

	size_t size = 0;

	while (*cc != '\0')
	{
		if (*cc < 128) //和ASCII一致
		{
			*ctemp = *(reinterpret_cast<const char *>(cc));
			ctemp++;
			size++;

			cc++;
			
			continue;
		}
		else if (*cc >0x80 && *cc < 0xff) //0x81-0xfe,  0x40,0xfe
		{				//第一字节在GBK内
			auto b1 = *cc;
			cc++;

			auto b2 = *cc;
			if (b2 > 0x39 && b2 < 0xff) //第二字节也在GBK内
			{
				auto utf8 = gbktable[b1 - 0x81][b2 - 0x40];
				auto len = utf8[0];

				memcpy(ctemp, utf8 + 1, len);

				ctemp += len;
				size += len;

				cc++;

				continue;
			}

		}

		//错误字符
		{


			auto err = gbktable[0][0];
			auto len = err[0];

			memcpy(ctemp, err + 1, len);

			ctemp += len;
			size += len;
			cc++;

		}

	}

	*ctemp = '\0';

	return size;
}


#endif // !CFL_CONVERTUTF_H
