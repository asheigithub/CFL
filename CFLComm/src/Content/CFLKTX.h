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
			字节顺序包含写为32位整数数0x04030201。 
			如果该文件是小端则这被表示为字节0×01 0×02 0×03 0×04。 
			如果该文件是Big Endian那么这表示为字节0×04 0×03 0×02 0×01。 
			当读取字节顺序为32位整数的结果为0x04030201则文件的字节顺序相匹配的读取该文件的程序的字节顺序，并没有转换是必要的。 
			当读取字节序作为一个32位的整数的结果为0x01020304然后该文件的字节顺序是读取该文件的程序的字节序相反，
			在这种情况下，程序读取文件必须尾数转换所有头字节到的字节序该程序（即小端程序必须转换大端和大端程序必须转换成小端）。 
			*/
			unsigned int endianness = 0x04030201;
			/*
			对于压缩纹理，glType必须等于0。对于未压缩的纹理，glType指定type传递给参数glTex{,Sub}Image*D通常是从一个值表8.2中的OpenGL 4.4规范[OPENGL44]（UNSIGNED_BYTE，UNSIGNED_SHORT_5_6_5，等等。） 
			*/
			unsigned int glType;
			/*
			glTypeSize指定当需要存储在文件中的纹理数据字节序变换，应使用的数据类型的大小。 如果glType不为0，这应该是在对应于glType字节大小。 纹理数据，不依赖于平台字节序，包括压缩的纹理数据，glTypeSize必须等于1。
			*/
			unsigned int glTypeSize;
			/*
			对于压缩纹理，glFormat必须等于0。对于未压缩的纹理，glFormat指定format传递给参数glTex{,Sub}Image*D通常是从一个值表8.3中的OpenGL 4.4规范[OPENGL44]（RGB，RGBA， BGRA等）
			*/
			unsigned int glFormat;
			/*
			为压缩纹理，glInternalFormat必须等于压缩内部格式，通常从其中一个值表8.14了OpenGL 4.4说明书的[OPENGL44]。
			对于未压缩的纹理，glInternalFormat指定internalformat传递给参数glTexStorage*D或glTexImage*D通常的尺寸是内部格式从一个表8.12和8.13对OpenGL 4.4规范[OPENGL44]。 的尺寸的格式，应选择以匹配位深度提供的数据。  glInternalFormat加载压缩和未压缩的纹理时，装载到不支持尺寸格式，例如未拉伸的OpenGL ES 2.0上下文，其中上下文时除外使用internalformat参数需要具有相同的值的format的参数。
			*/
			unsigned int glInternalFormat;
			/*
			对压缩和未压缩的纹理，glBaseInternalFormat指定纹理的基内部格式，
			通常从一个值的表8.11了OpenGL 4.4说明书的[OPENGL44]（RGB，RGBA，ALPHA等）。
			对于未压缩的纹理，该值将是相同的glFormat和用作internalformat加载到不支持尺寸格式，
			例如未拉伸的OpenGL ES 2.0上下文的上下文时的参数。
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

		//解析KTX文件
		//返回是否成功
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
