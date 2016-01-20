#include "Content/CFLMemoryStream.h"
#include "CFLEntry.h"
namespace cfl
{
	namespace content
	{
		struct memoryStream_data
		{
			memoryStream_data(const char* bytes, size_t len):
				bytes( reinterpret_cast<const unsigned char*>( bytes)),
				len(len),
				pos(0),
				isdisposed(false)
			{
				
			}

			~memoryStream_data()
			{
				
			}

			void dispose()
			{
				isdisposed = true;
			}

			const unsigned char* bytes;
			size_t len;

			int pos;
			bool isdisposed;
		}; 


		memoryStream::memoryStream(const char* bytes, size_t stpos, size_t len) :
			data(new memoryStream_data(bytes + (stpos <len ? stpos : len), len - (stpos <len ? stpos : len)))
		{
			if (stpos > len)
			{
				
			}
		}

		memoryStream::~memoryStream(){}


		size_t memoryStream::getLength()
		{
			return data->len;
		}

		int memoryStream::getPosition() const
		{
			return data->pos;
		}

		void  memoryStream::setPosition(int value)
		{
			data->pos = value;
		}

		void memoryStream::dispose()
		{
			data->dispose();
		}

		int unsigned memoryStream::read(unsigned char* bytes, int offset, int size)
		{
			CFL_ASSERT(!data->isdisposed);

			if (data->pos >= static_cast<int>(getLength()))
			{
				return 0;
			}

			if (size <= 0)
			{
				return 0;
			}

			if (data->pos + size > static_cast<int>(getLength()))
			{
				size = static_cast<int>(getLength()) - data->pos;
			}

			CFL_ASSERT(data->pos >= 0 && data->pos + size <= data->len);


			memcpy(bytes+offset, data->bytes + data->pos , size);

			data->pos += size;

			return size;
		}

		int memoryStream::readByte()
		{
			CFL_ASSERT(!data->isdisposed);

			CFL_ASSERT(data->pos >= 0);

			if (data->pos >= static_cast<int>(getLength()))
			{
				return -1;
			}

			auto buff = data->bytes[data->pos] ;
			++data->pos;
			return buff;
		}

		int memoryStream::seek(int offset, cfl::content::seekOrigin::SeekOrigin origin)
		{
			CFL_ASSERT(!data->isdisposed);

			switch (origin)
			{
			case cfl::content::seekOrigin::begin:
				setPosition(offset);
				break;
			case cfl::content::seekOrigin::current:
				setPosition(getPosition() + offset);
				break;
			case cfl::content::seekOrigin::end:
				setPosition(getLength() + offset);
				break;
			default:
				break;
			}

			return getPosition();
		}

	}
}
