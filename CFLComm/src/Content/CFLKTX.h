#ifndef CFL_KTX_H
#define CFL_KTX_H

#include <stdlib.h>
#include <vector>

namespace cfl
{
	namespace content
	{
		
		struct KtxHeader
		{
			
			KtxHeader()
			{
				static unsigned char data[12]  = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };
				for (size_t i = 0; i < 12; i++)
				{
					identifier[i] = static_cast<char>( data[i]);
				}
			}

			/*
			0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A
			*/
			char  identifier[12];
			
			/*
			�ֽ�˳�����дΪ32λ������0x04030201�� 
			������ļ���С�����ⱻ��ʾΪ�ֽ�0��01 0��02 0��03 0��04�� 
			������ļ���Big Endian��ô���ʾΪ�ֽ�0��04 0��03 0��02 0��01�� 
			����ȡ�ֽ�˳��Ϊ32λ�����Ľ��Ϊ0x04030201���ļ����ֽ�˳����ƥ��Ķ�ȡ���ļ��ĳ�����ֽ�˳�򣬲�û��ת���Ǳ�Ҫ�ġ� 
			����ȡ�ֽ�����Ϊһ��32λ�������Ľ��Ϊ0x01020304Ȼ����ļ����ֽ�˳���Ƕ�ȡ���ļ��ĳ�����ֽ����෴��
			����������£������ȡ�ļ�����β��ת������ͷ�ֽڵ����ֽ���ó��򣨼�С�˳������ת����˺ʹ�˳������ת����С�ˣ��� 
			*/
			unsigned int endianness = 0x04030201;
			/*
			����ѹ������glType�������0������δѹ��������glTypeָ��type���ݸ�����glTex{,Sub}Image*Dͨ���Ǵ�һ��ֵ��8.2�е�OpenGL 4.4�淶[OPENGL44]��UNSIGNED_BYTE��UNSIGNED_SHORT_5_6_5���ȵȡ��� 
			*/
			unsigned int glType;
			/*
			glTypeSizeָ������Ҫ�洢���ļ��е����������ֽ���任��Ӧʹ�õ��������͵Ĵ�С�� ���glType��Ϊ0����Ӧ�����ڶ�Ӧ��glType�ֽڴ�С�� �������ݣ���������ƽ̨�ֽ��򣬰���ѹ�����������ݣ�glTypeSize�������1��
			*/
			unsigned int glTypeSize;
			/*
			����ѹ������glFormat�������0������δѹ��������glFormatָ��format���ݸ�����glTex{,Sub}Image*Dͨ���Ǵ�һ��ֵ��8.3�е�OpenGL 4.4�淶[OPENGL44]��RGB��RGBA�� BGRA�ȣ�
			*/
			unsigned int glFormat;
			/*
			Ϊѹ������glInternalFormat�������ѹ���ڲ���ʽ��ͨ��������һ��ֵ��8.14��OpenGL 4.4˵�����[OPENGL44]��
			����δѹ��������glInternalFormatָ��internalformat���ݸ�����glTexStorage*D��glTexImage*Dͨ���ĳߴ����ڲ���ʽ��һ����8.12��8.13��OpenGL 4.4�淶[OPENGL44]�� �ĳߴ�ĸ�ʽ��Ӧѡ����ƥ��λ����ṩ�����ݡ�  glInternalFormat����ѹ����δѹ��������ʱ��װ�ص���֧�ֳߴ��ʽ������δ�����OpenGL ES 2.0�����ģ�����������ʱ����ʹ��internalformat������Ҫ������ͬ��ֵ��format�Ĳ�����
			*/
			unsigned int glInternalFormat;
			/*
			��ѹ����δѹ��������glBaseInternalFormatָ������Ļ��ڲ���ʽ��
			ͨ����һ��ֵ�ı�8.11��OpenGL 4.4˵�����[OPENGL44]��RGB��RGBA��ALPHA�ȣ���
			����δѹ����������ֵ������ͬ��glFormat������internalformat���ص���֧�ֳߴ��ʽ��
			����δ�����OpenGL ES 2.0�����ĵ�������ʱ�Ĳ�����
			*/
			unsigned int glBaseInternalFormat;
			/*
			The size of the texture image for level 0, in pixels. No rounding to block sizes should be applied for block compressed textures.
			For 1D textures pixelHeight and pixelDepth must be 0. For 2D and cube textures pixelDepth must be 0.
			*/
			unsigned int pixelWidth;
			/*
			The size of the texture image for level 0, in pixels. No rounding to block sizes should be applied for block compressed textures.
			For 1D textures pixelHeight and pixelDepth must be 0. For 2D and cube textures pixelDepth must be 0.
			*/
			unsigned int pixelHeight;
			/*
			The size of the texture image for level 0, in pixels. No rounding to block sizes should be applied for block compressed textures.
			For 1D textures pixelHeight and pixelDepth must be 0. For 2D and cube textures pixelDepth must be 0.
			*/
			unsigned int pixelDepth;
			/*
			numberOfArrayElements specifies the number of array elements. If the texture is not an array texture, numberOfArrayElements must equal 0.
			*/
			unsigned int numberOfArrayElements;
			/*
			numberOfFaces specifies the number of cubemap faces. For cubemaps and cubemap arrays this should be 6. For non cubemaps this should be 1. Cube map faces are stored in the order: +X, -X, +Y, -Y, +Z, -Z.
			*/
			unsigned int numberOfFaces;
			/*
			numberOfMipmapLevels must equal 1 for non-mipmapped textures. For mipmapped textures, it equals the number of mipmaps. Mipmaps are stored in order from largest size to smallest size. The first mipmap level is always level 0. A KTX file does not need to contain a complete mipmap pyramid. If numberOfMipmapLevels equals 0, it indicates that a full mipmap pyramid should be generated from level 0 at load time (this is usually not allowed for compressed formats).
			*/
			unsigned int numberOfMipmapLevels;
			/*
			An arbitrary number of key/value pairs may follow the header. This can be used to encode any arbitrary data. The bytesOfKeyValueData field indicates the total number of bytes of key/value data including all keyAndValueByteSize fields, all keyAndValue fields, and all valuePadding fields. The file offset of the first imageSize field is located at the file offset of the bytesOfKeyValueData field plus the value of the bytesOfKeyValueData field plus 4.
			*/
			unsigned int bytesOfKeyValueData;

		};


		
		struct DataSizeAndPos
		{
			DataSizeAndPos(size_t mip,size_t size,size_t pos):
				miplevel(mip), imageSize(size), pos(pos)
			{
				
			}

			size_t miplevel;
			size_t imageSize;
			size_t pos;
		};

		

		struct KTXFile
		{
			
			KtxHeader header;
			std::vector<DataSizeAndPos> datalist;

		};

		//����KTX�ļ�
		//�����Ƿ�ɹ�
		bool parseKTXFile(const char* data, size_t len, KTXFile* input);



		/*for each keyValuePair that fits in bytesOfKeyValueData
			UInt32   keyAndValueByteSize
			Byte     keyAndValue[keyAndValueByteSize]
			Byte     valuePadding[3 - ((keyAndValueByteSize + 3) % 4)]
		end

		for each mipmap_level in numberOfMipmapLevels*
			UInt32 imageSize;
			for each array_element in numberOfArrayElements*
				for each face in numberOfFaces
					for each z_slice in pixelDepth*
						for each row or row_of_blocks in pixelHeight*
							for each pixel or block_of_pixels in pixelWidth
								Byte data[format - specific - number - of - bytes] **
							end
						end
					end
					Byte cubePadding[0 - 3]
				end
			end
			Byte mipPadding[3 - ((imageSize + 3) % 4)]
		end*/
		//* Replace with 1 if this field is 0.

		//** Uncompressed texture data matches a GL_UNPACK_ALIGNMENT of 4.*/


	}
}






#endif // !CFL_KTX_H
