#ifndef CFL_MEMORYSTREAM_H
#define CFL_MEMORYSTREAM_H

#include "CFLStream.h"
#include <memory>

namespace cfl
{
	namespace content
	{

		struct memoryStream_data;
		

		//���ֽ�������Ϊֻ���ڴ���ʹ��
		class memoryStream : public cfl::content::stream
		{
		public:
			memoryStream(const char* bytes,size_t stpos, size_t len);

			~memoryStream() override final;

			size_t getLength() override final;

			int getPosition() const override final;

			void   setPosition(int value) override final;

			void dispose() override final;

			int unsigned read(unsigned char* bytes, int offset, int size) override final;

			int readByte() override final;

			int seek(int offset, cfl::content::seekOrigin::SeekOrigin origin) override final;

			//�Ƿ��Ѵ���
			bool isDisposed() const;
		private:
			std::shared_ptr<memoryStream_data> data;
		};


	}
}



#endif // !CFL_MEMORYSTREAM_H
