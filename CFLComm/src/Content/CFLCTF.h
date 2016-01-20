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
			//ֱ�Ӵ�PNGת������
			rgba = 1,
			//����ktx�ļ�
			ktx=2,

		};

		//������δ���͸��ͨ��
		enum CTFAlphaType :unsigned int
		{
			//�����Դ�alphaͨ��
			alphachannal =1,
			//���·ֲ�
			atlas=2,
			//����һ��������͸��ͨ��
			separate=3
		};
		//˵�������Ƿ���͸������ 
		enum CTFHasAlpha :unsigned int
		{
			//��͸��
			transparent=1,
			//û͸��
			notransparent=2

		};


		//�ļ�ͷ
		struct CTFHeader
		{
			CTFHeader(){ signature[0] = 'C'; signature[1] = 'T'; signature[2] = 'F'; signature[3] = 0; }

			char  signature[4];

			 unsigned int endiness = 0x04030201;

			 unsigned int  version = 110;

			 CTFContentType contentType;
			 CTFAlphaType   alphaType;		//��δ���͸��
			 CTFHasAlpha    hasAlpha;		//�Ƿ�͸��

			 unsigned int originImageWidth;//ԭʼͼ��Ŀ��
			 unsigned int originImageHeight;//ԭʼͼ��ĸ߶�

			 unsigned int width;           //�����
			 unsigned int height;		//�����
			 unsigned int stride;		//ɨ�����ֽ���

			 unsigned int length;       //�����ֽڳ���
			 unsigned int separateLength;  //����͸�������ֽڳ���
		};

		//����CTF�ļ�ͷ
		//�����Ƿ�ɹ�
		bool parseCTFHeader(const char* data,size_t len,CTFHeader* input);
		

	}
}

#endif // !CFL_CTF_H
