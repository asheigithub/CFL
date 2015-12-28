// FreeTypeTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//gdi+
#include <comdef.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma  comment(lib, "gdiplus.lib")

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>


#include <ft2build.h>
#include FT_FREETYPE_H 
#include <freetype/ftglyph.h>


#include "huffman.h"
#include <fstream>


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

void MakeImage(FT_Face face,FT_ULong charcode,const wchar_t* fontname);

int _tmain(int argc, _TCHAR* argv[])
{
	//����huffman

	auto teststr = "cfl::content::unHuffmanCompress(hfile.data.get(), hfile.getTotalFileLength())";

	//cfl::content::test(teststr, strlen(teststr));

	//auto hfile = cfl::content::huffmanCompress(teststr, strlen(teststr));

	//auto un = cfl::content::unHuffmanCompress(hfile.data.get(), hfile.getTotalFileLength());

	//ѹ������
	std::ifstream fstm;
	{
		fstm.open("E:/CFL/Debug/testreadfile/ktx888.ktx", std::ios::binary);
		fstm.seekg(0, std::ios::end);
		auto end_pos = fstm.tellg();

		fstm.seekg(0, std::ios::beg);
		auto size = end_pos.seekpos();

		char* filebuff = new char[size];
		fstm.read(filebuff, size);
		fstm.close();

		auto compressed = cfl::content::huffmanCompress(filebuff, size);
		delete[] filebuff;
	
		std::ofstream fsave;
		fsave.open("E:/CFL/Debug/testreadfile/hufftex.dat", std::ios::binary);
		fsave.write(compressed.data.get(), compressed.getTotalFileLength());
		fsave.close();
	}
	//��ѹ����
	fstm.open("E:/CFL/Debug/testreadfile/hufftex.dat", std::ios::binary);
	{
		fstm.seekg(0, std::ios::end);
		auto end_pos = fstm.tellg();

		fstm.seekg(0, std::ios::beg);
		auto size = end_pos.seekpos();

		char* filebuff = new char[size];
		fstm.read(filebuff, size);
		fstm.close();

		auto tick = GetTickCount();
		/*for (size_t i = 0; i < 20; i++)
		{*/
		auto uncompress = cfl::content::unHuffmanCompress(filebuff, size);

		//}
		
		auto use = GetTickCount() - tick;
		std::cout << " use tick " << use << std::endl;

		delete[] filebuff;

		std::ofstream fsave;
		fsave.open("E:/CFL/Debug/testreadfile/hufftex.ktx", std::ios::binary);
		fsave.write(uncompress.data.get(), uncompress.dataSize);
		fsave.close();

	}
	
	char key;
	std::cin >> key;

	return 0;

	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	FT_Library library; /* ��ľ�� */
	FT_Face face; /* face����ľ�� */

	auto error = FT_Init_FreeType(&library);
	if (error) { std::cout << "��ʼ��ʧ��"; }


	const wchar_t* fontname = L"msyh";

	error = FT_New_Face(library,
		"C:/Windows/Fonts/msyh.ttc", //'ARIALN.TTF'
		0,
		&face);

	if (error == FT_Err_Unknown_File_Format)
	{
		std::cout << " ���Դ򿪺Ͷ�����ļ�������֧�����������ʽ" << std::endl;
	}
	else if (error)
	{
		std::cout << " �����Ĵ�������ζ����������ļ����ܴ򿪺Ͷ������߼򵥵�˵������..." << std::endl;
	}

	/*һ��face���������face��ȫ��ȫ��������Ϣ��ͨ���ģ���Щ���ݿ���ͨ���ֱ��ѯ�����ֱ�ӷ��ʣ�����face->num_glyphs��

	FT_FaceRec�ṹ���������˿����ֶε������б�������������ϸ�������е�ĳЩ��

	Num_glyphs
	���ֵ�����˸�����face�п��õ�����(glyphs)��Ŀ������˵��һ�����ξ���һ���ַ�ͼ�񡣵�����һ������һ���ַ����롣

	Flags
	һ��32λ����������һЩ��������face���Ե�λ��ǡ����磬���FT_FACE_FLAG_SCALABLE����ָʾ��face�������ʽ�ǿ��������Ҹ�����ͼ�������Ⱦ���κ��ַ����سߴ硣Ҫ�˽�face��ǵĸ�����Ϣ�����Ķ���FreeType 2 API �ο�����

	Units_per_EM
	����ֶ�ֻ�Կ�������ʽ��Ч����������ʽ��������Ϊ0����ָʾ��EM�����ǵ����嵥λ�ĸ�����

	Num_fixed_size
	����ֶθ����˵�ǰface��Ƕ���λͼ�ĸ���������˵��һ��strike����ĳһ�ض��ַ����سߴ��µ�һϵ������ͼ�����磬һ������face���԰������سߴ�Ϊ10��12��14��strike��Ҫע����Ǽ�ʹ�ǿ������������ʽҰ���԰���Ƕ���λͼ��

	Fixed_sizes
	һ��ָ��FT_Bitmap_Size��Ա��ɵ������ָ�롣ÿһ��FT_Bitmap_Sizeָʾface�е�ÿһ��strike��ˮƽ�ʹ�ֱ�ַ����سߴ硣
	ע�⣬ͨ����˵���ⲻ��λͼstrike�ĵ�Ԫ�ߴ硣*/



	//�����ض�face�����ַ���С��ص���Ϣ��FreeType 2ʹ��size���������졣���磬���ַ���СΪ12��ʱ��ʹ��һ��size������1 / 64����Ϊ��λ����ĳЩ�����ascender�������ָ߶ȣ���ֵ��

	//	��FT_New_Face���������ݱ����ã������Զ���face���½�һ��size���󣬲����ء���size�������ͨ��face->sizeֱ�ӷ��ʡ�

	//	ע�⣺һ��face�������ͬʱ����һ������size���󣬵�ֻ�к��ٳ���Ա��Ҫ�õ�������ܣ���������Ǿ����򻯸�API�������磬ÿ��face����ֻӵ��һ��size���󣩵����������������Ȼͨ�����ӵĺ����ṩ��

	//	��һ���µ�face������ʱ�����ڿ����������ʽ��size����Ĭ��ֵΪ�ַ���Сˮƽ�ʹ�ֱ��Ϊ10���ء����ڶ��������ʽ�������С��δ����ģ�������������װ��һ������ǰ���ø�ֵ��ԭ��

	//	ʹ��FT_Set_Char_Size��ɸù��ܡ�������һ�����ӣ�����һ��300x300dpi�豸�ϰ��ַ���С����Ϊ16pt��

	std::cout << "units_per_EM " << face->units_per_EM << std::endl;

	//error = FT_Set_Char_Size(
	//	face, /* face����ľ�� */
	//	0, /* ��1/64��Ϊ��λ���ַ���� */
	//	16 * 64, /* ��1/64��Ϊ��λ���ַ��߶� */
	//	300, /* �豸ˮƽ�ֱ��� */
	//	300); /* �豸��ֱ�ֱ��� */

	/*ע�⣺

		* �ַ���Ⱥ͸߶���1 / 64��Ϊ��λ��ʾ��һ������һ��1 / 72Ӣ���������롣ͨ�����ⲻ����һ�����ء�

		* �豸��ˮƽ�ʹ�ֱ�ֱ�����ÿӢ�����(dpi)Ϊ��λ��ʾ����ʾ�豸������ʾ�����ĳ���ֵΪ72dpi��96dpi������ֱ������������ַ����������ַ����ش�С�ġ�

		* �ַ����Ϊ0��ζ�š����ַ��߶���ͬ�����ַ��߶�Ϊ0��ζ�š����ַ������ͬ�������������������ζ��ָ����һ�����ַ���Ⱥ͸߶ȡ�

		* ˮƽ��ֱ�ֱ���Ϊ0ʱ��ʾʹ��Ĭ��ֵ72dpi��

		* ��һ��������face����ľ��������size����ġ�

		������������Ӧ�ַ���ȡ��߶Ⱥ��豸�ֱ��ʵ��ַ����ش�С��Ȼ������������Լ�ָ�����ش�С������Լ򵥵ص���FT_Set_Pixel_Sizes������������
*/

	error = FT_Set_Pixel_Sizes(
		face, /* face������ */
		0, /* ���ؿ�� */
		128); /* ���ظ߶� */

	//ע�����������������ش����롣ͨ��������ᷢ���ڳ��ԶԶ��������ʽ����FNT��PCF�����ò���face->fixed_size�����е����سߴ�ֵ��

	/*ͨ����һ��Ӧ�ó�����ͨ���ַ�����װ����������ͼ���ַ�����һ���ض������д�����ַ�����ֵ�����磬�ַ���64������ASCII�����еġ�A����

		һ��face�������һ�������ַ���(charmap)���ַ���������ת���ַ��뵽���������ġ����磬�ܶ�TrueType������������ַ���һ������ת��Unicode�ַ��뵽����������
		��һ������ת��Apple Roman���뵽��������������������ȿ�������Windows��ʹ��Unicode����Macintosh��ʹ��Apple Roman����ͬʱҪע�⣬һ���ض����ַ������û�и��������������ȫ�����Ρ�

		���½�һ��face����ʱ����Ĭ��ѡ��Unicode�ַ����������û����Unicode�ַ���
		
		FreeType�᳢�����������Ļ�����ģ��һ����ע�⣬����������ǲ���׼����ôģ����ַ����п�����©ĳЩ���Ρ�����ĳЩ���壬������������;ɵ�������д���壬Unicodeģ���ǲ����ܵġ�

		���ǽ����Ժ��������Ѱ��face���ض����ַ����������Ǽ���face��������һ��Unicode�ַ���
		�����ڵ���FT_New_Faceʱ�Ѿ���ѡ�С�����ʹ��FT_Get_Char_Index��һ��Unicode�ַ���ת��Ϊ����������������ʾ��

		glyph_index = FT_Get_Char_Index(face, charcode);

		�����������face�ﱻѡ�е��ַ����в�����������ַ����Ӧ���������������û���ַ���ѡ�У���������򵥵ķ����ַ��롣

		ע�⣬���������FreeType�к��еĲ����ش�����ĺ����е�һ����Ȼ������һ���ض����ַ�����face��û������ͼ�񣬺�������0��
		����Լ��������Ӧһ�����������ͼ��D�Dȱʧ���Σ�ͨ������ʾһ�����һ���ո�


		һ������������������������װ�ض�Ӧ������ͼ���ڲ�ͬ������������ͼ��洢Ϊ��ͬ�ĸ�ʽ��
		���ڹ̶��ߴ������ʽ����FNT����PCF��ÿһ��ͼ����һ��λͼ�����ڿ����������ʽ����TrueType����Type1��
		ʹ����Ϊ����(outlines)��ʸ����״������ÿһ�����Ρ�
		һЩ�����ʽ�����и������;������ʾ���Σ���MetaFont�D�D�������ʽ����֧�֣���
		���˵ģ�FreeType2���㹻������ԣ�����ͨ��һ���򵥵�API֧���κ����͵����θ�ʽ��

		����ͼ��洢��һ���ر�Ķ���D�D���β�(glyph slot)�С�
		������������ʾ�ģ�һ�����β�ֻ��һ���򵥵�������
		��һ��ֻ������һ������ͼ�񣬿�����λͼ������������������������
		ÿһ��face������һ�����β۶��󣬿���ͨ��face->glyph�����ʡ�
		�����ֶ���FT_GlyphSlotRec�ṹ���ĵ��н����ˡ�

		ͨ������FT_Load_Glyph��װ��һ������ͼ�����β��У����£�

*/
	
	

	/*load_flags��ֵ��λ��־���ϣ�������ָʾĳЩ��������ġ���Ĭ��ֵ��FT_LOAD_DEFAULT��0��

		����������跨��face��װ�ض�Ӧ������ͼ��

		* ����ҵ�һ����Ӧ�����κ����سߴ��λͼ����ô�����ᱻװ�ص����β��С�Ƕ���λͼ���Ǳ�ԭ����ͼ���ʽ����װ�ء�
		
		��Ϊ���Ǽٶ���һ�����Σ����и��������İ汾���������FT_LOAD_NO_BITMAP��־���ı䡣

		* ���򣬽�װ��һ�������ε�ԭ��ͼ�񣬰�����������ǰ�����سߴ磬���Ҷ�Ӧ��TrueType��Type1��Щ��ʽ��Ҳ�����hinted������

		�ֶ�face->glyph->format���������β��д洢������ͼ��ĸ�ʽ��
		
		�������ֵ����FT_GLYPH_FORMAT_BITMAP��
		
		�����ͨ��FT_Render_Glyph����ֱ��ת��Ϊһ��λͼ�����£�

*/
	

	/*render_mode������һ��λ��־���ϣ�����ָʾ�����Ⱦ����ͼ��
	
	������ΪFT_RENDER_MODE_NORMAL��Ⱦ��һ���������Ŀ����(256���Ҷ�)λͼ��
		
	����Ĭ�����������������ɺڰ�λͼ������ʹ��FT_RENDER_MODE_MONO��־��
*/


	/*һ����������һ������ͼ���λͼ�������ͨ��glyph->bitmap(һ���򵥵�λͼ������)ֱ�ӷ��ʣ�
	
	ͬʱ��glyph->bitmap_left��glyph->bitmap_top��ָ����ʼλ�á�

		Ҫע�⣬bitmap_left�Ǵ�����λͼ��ǰ��λ�õ�����߽��ˮƽ���룬
		
		��bitmap_top�Ǵӱ�λ�ã�λ�ڻ��ߣ�����߽߱�ô�ֱ���롣��ô��������ָʾһ�����ϵľ��롣

		��һ���ֽ��������β����ݵĸ���ϸ�ڣ��Լ���η����ض���������Ϣ��������������*/
/*

	������slot->advance���ӱ�λ�ã�slot->advance�������εĲ������
	��Ҳ����ͨ����˵���߸�(escapement)��������ʸ�������ص�1 / 64Ϊ��λ��ʾ��������ÿһ�ε�����ɾ��Ϊ�������ء�
*/
	//MakeImage(face, 'A');


	FT_ULong  charcode;
	FT_UInt   gindex;

	auto use_kerning = FT_HAS_KERNING(face);
	FT_ULong previous_charcode = 0;
	FT_UInt  previous_glyph = 0;

	

	charcode = FT_Get_First_Char(face, &gindex);
	while (gindex != 0)
	{
		//ö�����е�charcode
			charcode = FT_Get_Next_Char(face, charcode, &gindex);



			MakeImage(face, charcode,fontname);


			auto glyph_index = FT_Get_Char_Index(face, charcode);  //0x0E39

			
			if (use_kerning && previous_glyph > 0)
			{
				FT_Vector delta;

				FT_Get_Kerning(face, previous_glyph, glyph_index,
					FT_KERNING_DEFAULT, &delta);

				std::cout << "kerning: " << 
					std::hex << std::setw(4) 
					<< std::setfill('0') << previous_charcode << " " << charcode
					<< std::dec<< " x:" << delta.x <<" y:" << delta.y << std::endl   ;

			}
			previous_glyph = glyph_index;
			previous_charcode = charcode;
	}



	char a;
	std::cin >> a;
	
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return 0;
}





int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}


void MakeImage(FT_Face face, FT_ULong charcode,const wchar_t* fontname )
{
	auto glyph_index = FT_Get_Char_Index(face, charcode);  //0x0E39

	auto error = FT_Load_Glyph(
		face, /* face����ľ�� */
		glyph_index, /* �������� */
		FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP); /* װ�ر�־���ο����� */
	
	error = FT_Render_Glyph(face->glyph, /* ���β� */
		FT_RENDER_MODE_NORMAL); /* ��Ⱦģʽ */

	FT_GlyphSlot slot;
	slot = face->glyph;

	std::cout << "advance.x " << (slot->advance.x / 64) << std::endl;
	std::cout << "advance.y " << (slot->advance.y / 64) << std::endl;

	if (charcode == 0x0e31)
	{
		std::cout << slot->advance.x;
	}


	
	
	
	auto bitmap = slot->bitmap;
	auto buffer = bitmap.buffer;

	int imageWidth = 128;// slot->advance.x / 64;//Ҫ���ɵ�ͼƬ�Ĵ�С
	int imageHeight = 128;  //face->size->metrics.height / 64;



	int detx = (imageWidth - bitmap.width) / 2;// -slot->bitmap_left;
	int dety = (imageHeight - bitmap.rows) / 2;// -slot->bitmap_top;

	int fontcenterx = detx - slot->bitmap_left;
	int fontcentery = dety + slot->bitmap_top;

	
	{
		Bitmap image(imageWidth, imageHeight);
		for (size_t j = 0; j < imageHeight; j++)
		{
			for (size_t i = 0; i < imageWidth; i++)
			{
				
				auto c = i - detx;
				auto r = j - dety;

				if (c >= 0 && r >= 0 && c < bitmap.width && r < bitmap.rows)
				{
					auto cc = buffer[r * bitmap.width + c];
					image.SetPixel(i, j, Color(cc, cc, cc));
				}
				else
				{
					image.SetPixel(i, j, Color(0, 0, 0));
				}

				if ( abs( (int)( i - fontcenterx))<3 && abs( (int)(j - fontcentery))<3 )
				{
					image.SetPixel(i, j, Color(255, 0, 0));
				}

			}
		}


		CLSID  encoderClsid;
		INT    result;
		WCHAR  strGuid[39];

		result = GetEncoderClsid(L"image/png", &encoderClsid);

		if (result < 0)
		{
			printf("The PNG encoder is not installed.\n");
		}
		else
		{
			std::wostringstream wstr;
			wstr << L"E:/CFL/Tools/FreeTypeTest/outputpng/" << fontname << "/" << std::hex << std::setw(4) << std::setfill(L'0') << charcode << ".png";

			image.Save(wstr.str().c_str(), &encoderClsid);
		}

	}


}
