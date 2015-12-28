#include "Content/CFLCTF.h"
#include "CFLEntry.h"
#include "Content/CFLBinaryReader.h"

namespace cfl
{
	namespace content
	{
		bool parseCTFHeader(const char* data, size_t len, CTFHeader* input)
		{
			if (len <= sizeof(CTFHeader)) //文件长度不够
			{
				return false;
			}

			BinaryReader br = BinaryReader(data, 0, len, CFLContext::getCurrentInstance()->capability->getEndian());
			
			char signature[4];
			signature[0] = br.readByte();
			signature[1] = br.readByte();
			signature[2] = br.readByte();
			signature[3] = br.readByte();
			
			if (signature[0] != 'C' || signature[1] != 'T' || signature[2] != 'F' || signature[3] != 0)
			{
				return false;
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
			input->version = br.readUnsignedInt();
			input->contentType = static_cast<content::CTFContentType>(br.readUnsignedInt());
			input->alphaType = static_cast<content::CTFAlphaType>(br.readUnsignedInt());
			input->hasAlpha = static_cast<content::CTFHasAlpha>(br.readUnsignedInt());

			input->originImageWidth = br.readUnsignedInt();
			input->originImageHeight = br.readUnsignedInt();
			input->width = br.readUnsignedInt();
			input->height = br.readUnsignedInt();
			input->stride = br.readUnsignedInt();

			input->length = br.readUnsignedInt();
			input->separateLength = br.readUnsignedInt();

			

			return true;
		}

	}
}
