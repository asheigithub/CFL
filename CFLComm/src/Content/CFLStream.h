#ifndef CFLSTREAM_H
#define CFLSTREAM_H
#include <stdlib.h>  
namespace cfl
{
	namespace content
	{
		namespace seekOrigin
		{
			enum SeekOrigin
			{
				begin,//指定流的开头。
				current,//指定流内的当前位置。
				end//指定流的结尾。
			};
		}


		/*
		提供字节序列的一般视图。这是一个抽象类。
		*/
		class stream
		{
		public:
			virtual ~stream()=default;

			//当在派生类中重写时，获取流长度（以字节为单位）。
			virtual size_t getLength() = 0;

			//当在派生类中重写时，获取或设置当前流中的位置。
			virtual int getPosition() const = 0;
			//当在派生类中重写时，获取或设置当前流中的位置。
			virtual void   setPosition(int value) = 0;

			virtual void dispose() = 0;
			/*
			当在派生类中重写时，从当前流读取字节序列，并将此流中的位置提升读取的字节数。
			buffer
				字节数组。此方法返回时，该缓冲区包含指定的字符数组，该数组的 offset 和 (offset + count -1) 之间的值由从当前源中读取的字节替换。
			offset
				buffer 中的从零开始的字节偏移量，从此处开始存储从当前流中读取的数据。
			count
				要从当前流中最多读取的字节数。
			return
				读入缓冲区中的总字节数。如果很多字节当前不可用，则总字节数可能小于请求的字节数；如果已到达流结尾，则为零 (0)。
			*/
			virtual unsigned int read(unsigned char* buffer,int offset,int size )=0;

			//从流中读取一个字节，并将流内的位置向前提升一个字节，或者如果已到达流结尾，则返回 -1。
			virtual int readByte()=0;

			/*
			当在派生类中重写时，设置当前流中的位置。
			offset
				相对于 origin 参数的字节偏移量。
			origin
				SeekOrigin 类型的值，指示用于获取新位置的参考点。
			返回值
				Type: int
				当前流中的新位置。
			*/
			virtual int seek(int offset, seekOrigin::SeekOrigin origin) = 0;


			


		};


	}
}


#endif // !CFLSTREAM_H
