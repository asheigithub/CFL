#include "Content/CFLBinaryReader.h"

#include "CFLEntry.h"

namespace cfl
{
	namespace content
	{
		struct binaryreader_indata
		{
			binaryreader_indata(
				const char* data,
				size_t stpos,
				size_t length,
				const Endian endian
				) :data(data),
				stpos(stpos),
				length(length),
				
				offset(0),
				endian(endian),
				machineEndian( CFLContext::getCurrentInstance()->capability->getEndian()  )
			{
				
			}

			const char* data;
			size_t stpos;
			size_t length;

			Endian endian;

			Endian machineEndian;

			size_t offset;

			const char* getptr()
			{
				return data + stpos+ offset;
			}
		};


		BinaryReader::BinaryReader(const char* bytes, 
			size_t stpos, 
			size_t length, 
			const Endian endian
			) :indata(new binaryreader_indata(bytes,stpos,length,endian))
		{
			
		}
		BinaryReader::~BinaryReader()
		{
		}

		Endian BinaryReader::getEndian() const
		{
			return indata->endian;
		}
		void   BinaryReader::setEndian(const Endian value)
		{
			indata->endian = value;
		}

		size_t BinaryReader::getPosition() const
		{
			return indata->offset;
		}
		void BinaryReader::setPosition(size_t offset)
		{
			indata->offset = offset;
		}


		char BinaryReader::readByte() const
		{
			if (indata->offset + 1 > indata->length)
			{
				throw new EOFException();
			}

			char ret;
			memcpy(&ret, indata->getptr(), 1);
			indata->offset += 1;

			return ret;

		}

		void BinaryReader::readBytes(char* dst, size_t len) const
		{
			if (indata->offset + len > indata->length)
			{
				throw new EOFException();
			}

			memcpy(dst, indata->getptr(), len);
			indata->offset += len;
		}

		int BinaryReader::readInt() const
		{
			if (indata->offset+4 > indata->length)
			{
				throw new EOFException();
			}

			int ret;
			memcpy(&ret, indata->getptr() , 4);

			indata->offset += 4;

			if (indata->endian != indata->machineEndian)
			{
				//反转字节序
				union {
					int i;
					char c[4];
				} u, r;

				u.i = ret;
				r.c[0] = u.c[3];
				r.c[1] = u.c[2];
				r.c[2] = u.c[1];
				r.c[3] = u.c[0];

				return r.i;
			}

			
			return ret;
		}


		unsigned int BinaryReader::readUnsignedInt() const
		{
			if (indata->offset + 4 > indata->length)
			{
				throw new EOFException();
			}

			unsigned int ret;
			memcpy(&ret, indata->getptr(), 4);

			indata->offset += 4;

			if (indata->endian != indata->machineEndian)
			{
				//反转字节序
				union {
					unsigned int i;
					char c[4];
				} u, r;

				u.i = ret;
				r.c[0] = u.c[3];
				r.c[1] = u.c[2];
				r.c[2] = u.c[1];
				r.c[3] = u.c[0];

				return r.i;
			}


			return ret;
		}


		short BinaryReader::readShort() const
		{
			if (indata->offset + 2 > indata->length)
			{
				throw new EOFException();
			}

			short ret;
			memcpy(&ret, indata->getptr(), 2);

			indata->offset += 2;

			if (indata->endian != indata->machineEndian)
			{
				//反转字节序
				union {
					short i;
					char c[2];
				} u, r;

				u.i = ret;
				r.c[0] = u.c[1];
				r.c[1] = u.c[0];
				

				return r.i;
			}


			return ret;
		}


		float BinaryReader::readFloat() const
		{
			if (indata->offset + 4 > indata->length)
			{
				throw new EOFException();
			}

			float ret;
			memcpy(&ret, indata->getptr(), 4);

			indata->offset += 4;

			if (indata->endian != indata->machineEndian)
			{
				//反转字节序
				union {
					float i;
					char c[4];
				} u, r;

				u.i = ret;
				r.c[0] = u.c[3];
				r.c[1] = u.c[2];
				r.c[2] = u.c[1];
				r.c[3] = u.c[0];
				
				return r.i;
			}


			return ret;
		}

		double BinaryReader::readDouble() const
		{
			if (indata->offset + 8 > indata->length)
			{
				throw new EOFException();
			}

			double ret;
			memcpy(&ret, indata->getptr(), 8);

			indata->offset += 8;

			if (indata->endian != indata->machineEndian)
			{
				//反转字节序
				union {
					double i;
					char c[8];
				} u, r;

				u.i = ret;
				r.c[0] = u.c[7];
				r.c[1] = u.c[6];
				r.c[2] = u.c[5];
				r.c[3] = u.c[4];
				r.c[4] = u.c[3];
				r.c[5] = u.c[2];
				r.c[6] = u.c[1];
				r.c[7] = u.c[0];

				return r.i;
			}


			return ret;
		}

		CFLString BinaryReader::readUTFBytes(size_t length) const
		{
			if (indata->offset + length > indata->length)
			{
				throw new EOFException();
			}

			char* buff = new char[length + 1];

			memcpy(buff, indata->getptr(), length);
			buff[length] = 0;


			indata->offset += length;

			CFLString ret= CFLString(buff,text::Encoding::utf8);

			delete buff;

			return ret;
		}


	}
}
