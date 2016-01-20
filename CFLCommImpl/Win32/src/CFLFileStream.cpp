#include "CFLFileStream.h"


#include <direct.h>  
#include <io.h>
#include "CFLAsyncFunc.h"


namespace cfl
{
	namespace file
	{
		
		static FILE* getFileHandle(void* filehandle)
		{
			return reinterpret_cast<FILE*>(filehandle);
		}

		
		fileStream::fileStream(void* filehandle, void* ext) :filehandle(filehandle)
			, _len(nullptr),
			_pos(0),
			//needUpdatePos(false),
			isdisposed(false),
			bufferSt(0),
			bufferEd(0),
			ext(nullptr)
		{
			
		}

		fileStream::~fileStream()
		{
			dispose();

			if (_len)
			{
				delete _len;
				_len = nullptr;
			}

		}

		size_t fileStream::getLength()
		{
			if (!_len)
			{
				CFL_ASSERT(!isdisposed);
				auto fp = getFileHandle(filehandle);

				if (fseek(fp, 0, SEEK_END) != 0)
				{
					LOGE("IO����ʧ��");
					throw new std::runtime_error("IO����ʧ��");
				}
				auto len = ftell(fp);
				_len = new size_t(len);

				//����֮ǰ��λ��
				if (fseek(fp, _pos, SEEK_SET) != 0)
				{
					LOGE("IO����ʧ��");
					throw new std::runtime_error("IO����ʧ��");
				}

			} 
			return *_len;
		}

		int fileStream::getPosition() const
		{
			return _pos;
		}

		void  fileStream::setPosition(int value)
		{
			CFL_ASSERT(!isdisposed);

			//if (_pos != value)
			{
				_pos = value;
				//needUpdatePos = true;
			}
		}

		void fileStream::dispose()
		{
			if (!isdisposed)
			{
				isdisposed = true;
				fclose(getFileHandle(filehandle));
			}
			
		}

		//���ļ������buffer
		static int fillData(unsigned char* buffer, size_t bufferlen, FILE* fp, size_t filepos, size_t len)
		{
			if (fseek(fp, filepos, SEEK_SET) != 0)
			{
				LOGE("IO����ʧ��");
				throw new std::runtime_error("IO����ʧ��");
			}

			size_t toread = FILESTREAM_BUFFSIZE;

			if (filepos + toread > len)
			{
				toread = len - filepos;
			}

			size_t readed = 0;

			while (toread>0)
			{
				auto readlen = fread(buffer + readed , sizeof(unsigned char), toread, fp);

				toread -= readlen;

				readed += readlen;
				if (readlen==0)
				{
					break;
				}
			}

			return readed;
		}



		int unsigned fileStream::read(unsigned char* bytes, int offset, int size)
		{
			CFL_ASSERT(!isdisposed);

			if (_pos >= static_cast<int>(getLength()))
			{
				return 0;
			}
			
			if (size <= 0)
			{
				return 0;
			}

			if (_pos + size > static_cast<int>(getLength()))
			{
				size = static_cast<int>(getLength()) - _pos;
			}

			if (size <= FILESTREAM_BUFFSIZE) //�������С����sizeʱ
			{

				if (!(_pos >= bufferSt && _pos + size <= bufferEd))
				{
					bufferSt = _pos - (FILESTREAM_BUFFSIZE - size) / 2;
					if (bufferSt < 0)
					{
						bufferSt = 0;
					}

					bufferEd = bufferSt+fillData(buffer,FILESTREAM_BUFFSIZE, getFileHandle(filehandle), bufferSt ,getLength() );
					
					CFL_ASSERT((_pos >= bufferSt && _pos + size <= bufferEd));

				}
				
				memcpy(bytes+offset, buffer + (_pos - bufferSt), size);

				_pos += size;

				return size;
			}
			else  //�������СС��sizeʱ                 
			{
				

				//auto fp = getFileHandle(filehandle);
				////if (needUpdatePos)
				//{
				//	if (fseek(fp, _pos, SEEK_SET) != 0)
				//	{
				//		LOGE("IO����ʧ��");
				//		throw new std::runtime_error("IO����ʧ��");
				//	}
				//	//needUpdatePos = false;
				//}

				//auto readlen = fread(bytes + offset, sizeof(unsigned char), size, fp);

				//bufferSt = _pos;
				//bufferEd = _pos + FILESTREAM_BUFFSIZE;
				//memcpy(buffer, bytes + offset, FILESTREAM_BUFFSIZE);

				//_pos += readlen;

				//return readlen;

				auto readlen = fillData(bytes + offset, size, getFileHandle(filehandle), _pos, getLength());
				//fread(bytes + offset, sizeof(unsigned char), size, fp);

				bufferSt = _pos;
				bufferEd = _pos + FILESTREAM_BUFFSIZE;
				memcpy(buffer, bytes + offset, FILESTREAM_BUFFSIZE);

				_pos += readlen;

				return readlen;


			}

		}




		int fileStream::readByte()
		{
			CFL_ASSERT(!isdisposed);

			if (_pos >= static_cast<int>( getLength()))
			{
				return -1;
			}

			if (_pos < bufferSt || _pos >= bufferEd) //����buffer
			{
				bufferSt = _pos - FILESTREAM_BUFFSIZE / 2;
				if (bufferSt < 0)
				{
					bufferSt = 0;
				}

				bufferEd =bufferSt+ fillData(buffer,FILESTREAM_BUFFSIZE, getFileHandle(filehandle), bufferSt,getLength());

			}

			auto buff = buffer[_pos - bufferSt];

			++_pos;

			return buff;
		}

		int fileStream::seek(int offset, cfl::content::seekOrigin::SeekOrigin origin)
		{
			CFL_ASSERT(!isdisposed);


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