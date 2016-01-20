#ifndef CFL_FILESTREAM_H
#define CFL_FILESTREAM_H

#include "Content/CFLStream.h"
#include <memory>

namespace cfl
{
	namespace file
	{
		#define FILESTREAM_BUFFSIZE 1024*64
		/*
		�ļ���
		*/
		class fileStream : public cfl::content::stream
		{
		public:
			fileStream(void* filehandle,void* ext);

			~fileStream() override final;

			size_t getLength() override final;

			int getPosition() const override final;
			
			void   setPosition(int value) override final;

			void dispose() override final;
			
			int unsigned read(unsigned char* bytes, int offset, int size) override final;

			int readByte() override final;

			int seek(int offset, cfl::content::seekOrigin::SeekOrigin origin) override final;

			//�Ƿ��Ѵ���
			bool isDisposed() const{ return isdisposed; }
		private:
			void* filehandle;

			void* ext;

			size_t* _len;

			int _pos;
			//bool needUpdatePos;

			bool isdisposed;



			//����
			unsigned char buffer[FILESTREAM_BUFFSIZE];
			//���忪ʼλ��
			int bufferSt;
			//�������λ��
			int bufferEd;

		};

	}
}


#endif // !CFL_FILESTREAM_H
