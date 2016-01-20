#ifndef CFL_CTF_H
#define CFL_CTF_H

#ifdef WIN32

#define ALIGN(a) __declspec(align(a))

#else

#define ALIGN(a) alignas(a)

#endif // WIN32

#include <stdlib.h>

#include "CFLKTX.h"

namespace cfl
{
	namespace content
	{
		enum CTFContentType :unsigned int
		{
			//直接从PNG转过来的
			rgba = 1,
			//包含ktx文件
			ktx=2,

		};

		//纹理如何处理透明通道
		enum CTFAlphaType :unsigned int
		{
			//纹理自带alpha通道
			alphachannal =1,
			//上下分层
			atlas=2,
			//用另一张纹理保存透明通道
			separate=3
		};
		//说明纹理是否有透明部分 
		enum CTFHasAlpha :unsigned int
		{
			//有透明
			transparent=1,
			//没透明
			notransparent=2

		};


		//文件头
		struct CTFHeader
		{
			CTFHeader(){ signature[0] = 'C'; signature[1] = 'T'; signature[2] = 'F'; signature[3] = 0; }

			char  signature[4];

			 unsigned int endiness = 0x04030201;

			 unsigned int  version = 110;

			 CTFContentType contentType;
			 CTFAlphaType   alphaType;		//如何处理透明
			 CTFHasAlpha    hasAlpha;		//是否透明

			 unsigned int originImageWidth;//原始图像的宽度
			 unsigned int originImageHeight;//原始图像的高度

			 unsigned int width;           //纹理宽
			 unsigned int height;		//纹理高
			 unsigned int stride;		//扫描行字节数

			 unsigned int length;       //数据字节长度
			 unsigned int separateLength;  //附加透明纹理字节长度
		};

		//解析CTF文件头
		//返回是否成功
		bool parseCTFHeader(const char* data,size_t len,CTFHeader* input);
		

	}
}

#endif // !CFL_CTF_H
