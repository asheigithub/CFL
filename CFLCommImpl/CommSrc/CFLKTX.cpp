#include "Content/CFLKTX.h"
#include "Content/CFLBinaryReader.h"
#include "CFLEntry.h"

namespace cfl
{
	namespace content
	{

		bool parseKTXFile(const char* data, size_t len, KTXFile* input)
		{
			if (len <= sizeof(KtxHeader)) //文件长度不够
			{
				return false;
			}

			BinaryReader br = BinaryReader(data, 0, len, CFLContext::getCurrentInstance()->capability->getEndian());

			char identifier[12];
			br.readBytes(identifier, 12);

			for (size_t i = 0; i < 12; i++)
			{
				if (identifier[i] != input->header.identifier[i])
				{
					return false;
				}
			}
			

			unsigned int endiness = br.readUnsignedInt();

			bool mathEndian;

			if (endiness == 0x04030201)
			{
				mathEndian = true;
			}
			else if (endiness == 0x01020304)
			{
				mathEndian = false;
			}
			else
			{
				return false;
			}

			if (!mathEndian)
			{
				if (CFLContext::getCurrentInstance()->capability->getEndian() == content::Endian::little_endian)
				{
					br.setEndian(content::Endian::big_endian);
				}
				else
				{
					br.setEndian(content::Endian::little_endian);
				}
			}
			input->header.glType = br.readUnsignedInt();
			input->header.glTypeSize = br.readUnsignedInt();
			input->header.glFormat = br.readUnsignedInt();
			input->header.glInternalFormat = br.readUnsignedInt();
			input->header.glBaseInternalFormat = br.readUnsignedInt();
			input->header.pixelWidth = br.readUnsignedInt();
			input->header.pixelHeight = br.readUnsignedInt();
			input->header.pixelDepth = br.readUnsignedInt();
			input->header.numberOfArrayElements = br.readUnsignedInt();
			input->header.numberOfFaces = br.readUnsignedInt();
			input->header.numberOfMipmapLevels = br.readUnsignedInt();
			input->header.bytesOfKeyValueData = br.readUnsignedInt();

			if (input->header.numberOfFaces != 1 && input->header.numberOfFaces != 6)
			{
				LOGE("ktx file error,must faces == (1 or 6)   \n");
				return false;
			}


			//校验数据长度
			br.setPosition(br.getPosition() + input->header.bytesOfKeyValueData);

			try
			{
				size_t numberOfMipmapLevels = input->header.numberOfMipmapLevels;
				if (numberOfMipmapLevels == 0){ numberOfMipmapLevels = 1; }

				for (size_t miplevel = 0; miplevel < numberOfMipmapLevels; miplevel++)
				{
					auto imageSize = br.readUnsignedInt();

					size_t numberOfArrayElements = input->header.numberOfArrayElements;
					if (numberOfArrayElements == 0){ numberOfArrayElements = 1; }

					for (size_t array_element = 0; array_element < numberOfArrayElements; array_element++)
					{
						for (size_t face = 0; face < input->header.numberOfFaces; face++)
						{
							input->datalist.push_back(DataSizeAndPos(miplevel,imageSize,br.getPosition()));

							br.setPosition(br.getPosition() + imageSize);

							/*cubePadding
								For non - array cubemap textures
								(any texture where numberOfFaces is 6 and numberOfArrayElements is 0) 
								cubePadding contains between 0 and 3 bytes 
								to ensure that the data in each face begins at a file offset 
								that is a multiple of 4. 
								In all other cases cubePadding is empty(0 bytes long).
							*/
							if (input->header.numberOfFaces == 6 && input->header.numberOfArrayElements == 0)
							{
								auto cubepos = br.getPosition();
								for (size_t cubePadding = 0; cubePadding < 3-( cubepos+3 ) % 4; cubePadding++)
								{
									br.readByte();
								}
							}
							
						}
					}

					/*mipPadding
						Between 0 and 3 bytes to make sure that all imageSize fields 
						are at a file offset that is a multiple of 4.
					*/
					auto mippos = br.getPosition();
					for (size_t padding = 0; padding < 3 - (mippos + 3) % 4; padding++)
					{
						br.readByte();
					}
					//Byte mipPadding[3 - ((imageSize + 3) % 4)]
				}

				if (br.getPosition() > len)
				{
					LOGE("ktx file error,length error \n");
					return false;
				}

			}
			catch (EOFException ex)
			{
				LOGE("ktx file error,length error \n");
				return false;
			}
			

			return true;
		}


	}
}
