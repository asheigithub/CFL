#include "Content/CFLBinaryReader.h"

#include "Content/CFLMemoryStream.h"
#include "CFLEntry.h"

namespace cfl
{
	namespace content
	{
		struct binaryreader_indata
		{
			binaryreader_indata(
				/*const char* data,
				size_t stpos,
				size_t length,*/

				const stream* basestream,

				const Endian endian,

				bool disposestream
				) :
				basestream( const_cast<stream*>( basestream)),
				/*data(data),
				stpos(stpos),
				length(length),
				
				offset(0),*/
				endian(endian),
				machineEndian( CFLContext::getCurrentInstance()->capability->getEndian()  ),

				disposestream(disposestream)
			{
				
			}

			~binaryreader_indata()
			{
				if (disposestream)
				{
					basestream->dispose();
					delete basestream;
				}
			}

			/*const char* data;
			size_t stpos;
			size_t length;*/

			stream* basestream;

			Endian endian;

			Endian machineEndian;

			bool disposestream;


			//size_t offset;

			/*const char* getptr()
			{
				return data + stpos+ offset;
			}*/
		};


		BinaryReader::BinaryReader(const char* bytes, 
			size_t stpos, 
			size_t length, 
			const Endian endian
			) :indata(new binaryreader_indata( new memoryStream(bytes,stpos,length), endian,true))
		{
			
		}

		BinaryReader::BinaryReader(const stream* stream, const Endian endian)
			: indata( new binaryreader_indata( stream,endian,false) )
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
			//return indata->offset;
			return indata->basestream->getPosition();
		}
		void BinaryReader::setPosition(size_t offset)
		{
			//indata->offset = offset;

			indata->basestream->setPosition(offset);

		}


		char BinaryReader::readByte() const
		{
			if (indata->basestream->getPosition() + 1 > static_cast<long>( indata->basestream->getLength()))
			{
				throw new EOFException();
			}

			auto byte = indata->basestream->readByte();
			CFL_ASSERT(byte >= 0);


			char ret;
			memcpy(&ret, &byte, 1);

			return ret;

		}

		void BinaryReader::readBytes(char* dst, size_t len) const
		{
			/*if (indata->offset + len > indata->length)
			{
				throw new EOFException();
			}

			memcpy(dst, indata->getptr(), len);
			indata->offset += len;*/

			if (indata->basestream->getPosition() + static_cast<long>(len) > static_cast<long>(indata->basestream->getLength()))
			{
				throw new EOFException();
			}

			indata->basestream->read( reinterpret_cast<unsigned char*>( dst), 0, len);
		}

		int BinaryReader::readInt() const
		{
			/*if (indata->offset+4 > indata->length)
			{
				throw new EOFException();
			}*/

			if (indata->basestream->getPosition() + 4 > static_cast<long>(indata->basestream->getLength()))
			{
				throw new EOFException();
			}
			/*unsigned char buff[4];
			indata->basestream->read(buff, 0, 4);

			int ret;
			memcpy(&ret, buff , 4);*/

			int ret;
			indata->basestream->read( reinterpret_cast<unsigned char*>( &ret), 0, 4);

			//indata->offset += 4;

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
			/*if (indata->offset + 4 > indata->length)
			{
				throw new EOFException();
			}*/

			if (indata->basestream->getPosition() + 4 > static_cast<long>(indata->basestream->getLength()))
			{
				throw new EOFException();
			}
			///*unsigned char buff[4];
			//indata->basestream->read(buff, 0, 4);*/

			unsigned int ret;
			//memcpy(&ret, buff, 4);
			indata->basestream->read(reinterpret_cast<unsigned char*>(&ret), 0, 4);

			//indata->offset += 4;

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
			/*if (indata->offset + 2 > indata->length)
			{
				throw new EOFException();
			}*/

			if (indata->basestream->getPosition() + 2 > static_cast<long>(indata->basestream->getLength()))
			{
				throw new EOFException();
			}
			/*unsigned char buff[2];
			indata->basestream->read(buff, 0, 2);*/

			short ret;
			//memcpy(&ret, buff, 2);
			indata->basestream->read(reinterpret_cast<unsigned char*>(&ret), 0, 2);

			//indata->offset += 2;

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

		unsigned short BinaryReader::readUnsignedShort() const
		{
			/*if (indata->offset + 2 > indata->length)
			{
				throw new EOFException();
			}*/

			if (indata->basestream->getPosition() + 2 > static_cast<long>(indata->basestream->getLength()))
			{
				throw new EOFException();
			}
			/*unsigned char buff[2];
			indata->basestream->read(buff, 0, 2);*/

			unsigned short ret;
			//memcpy(&ret, buff, 2);
			indata->basestream->read(reinterpret_cast<unsigned char*>(&ret), 0, 2);
			//indata->offset += 2;

			if (indata->endian != indata->machineEndian)
			{
				//反转字节序
				union {
					unsigned short i;
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
			/*if (indata->offset + 4 > indata->length)
			{
				throw new EOFException();
			}*/

			if (indata->basestream->getPosition() + 4 > static_cast<long>(indata->basestream->getLength()))
			{
				throw new EOFException();
			}
			/*unsigned char buff[4];
			indata->basestream->read(buff, 0, 4);*/

			float ret;
			//memcpy(&ret, buff, 4);
			indata->basestream->read(reinterpret_cast<unsigned char*>(&ret), 0, 4);

			//indata->offset += 4;

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
			/*if (indata->offset + 8 > indata->length)
			{
				throw new EOFException();
			}*/

			if (indata->basestream->getPosition() + 8 > static_cast<long>(indata->basestream->getLength()))
			{
				throw new EOFException();
			}
			/*unsigned char buff[8];
			indata->basestream->read(buff, 0, 8);*/

			double ret;
			//memcpy(&ret, buff, 8);
			indata->basestream->read(reinterpret_cast<unsigned char*>(&ret), 0, 8);
			//indata->offset += 8;

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
			/*if (indata->offset + length > indata->length)
			{
				throw new EOFException();
			}*/

			if (indata->basestream->getPosition() + static_cast<long>(length) > static_cast<long>(indata->basestream->getLength()))
			{
				throw new EOFException();
			}
			

			char* buff = new char[length + 1];

			//memcpy(buff, indata->getptr(), length);
			buff[length] = 0;
			indata->basestream->read( reinterpret_cast<unsigned char*>( buff), 0, length);

			//indata->offset += length;

			CFLString ret= CFLString(buff,text::Encoding::utf8);

			delete buff;

			return ret;
		}


	}
}
