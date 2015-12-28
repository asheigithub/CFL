#ifndef CFL_BINARYREADER_H
#define CFL_BINARYREADER_H

#include <stdexcept>
#include "../CFLString.h"
#include <memory>
#include "CFLEndian.h"

namespace cfl
{
	namespace content
	{
		
		class EOFException :public std::runtime_error
		{
		public:
			EOFException() :runtime_error(
				"读取位置越界。")
			{
				//LOGE("%s\n",exception::what());
			}

		};

		
		struct binaryreader_indata;
		
		/*
		二进制读取器
		*/
		class BinaryReader
		{
		public:
			BinaryReader(const char* bytes,size_t stpos,size_t length,const Endian endian=Endian::big_endian);
			~BinaryReader();
			
			Endian getEndian() const;
			void   setEndian(const Endian value);


			/**
			* 获得指针的当前位置（以字节为单位）
			*/
			 size_t getPosition() const;
			 
			 /**
			 * 将文件指针的当前位置（以字节为单位）移动到指定位置。下一次调用读取方法时将在此位置开始读取.
			 */
			 void   setPosition( size_t offset);



			/**
			* 从文件流、字节流或字节数组中读取布尔值。读取单个字节，如果字节非零，则返回 true，否则返回 false。
			* @return	一个布尔值，如果字节不为零，则为 true，否则为 false。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			bool  readBoolean() const;

			/**
			* 从文件流、字节流或字节数组中读取带符号的字节。
			* @return	返回值的范围是从 -128 到 127。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			char readByte() const;

			/*
			从流中读取指定长度字节，填充到目标字节流
			*/
			void readBytes(char* dst,size_t len  ) const;


			/**
			* 从文件流、字节流或字节数组中读取 IEEE 754 双精度浮点数。
			* @return	一个 IEEE 754 双精度浮点数。
			* @throws	EOFError 没有足够的数据可供读取。
			*/
			double readDouble() const;

			/**
			* 从文件流、字节流或字节数组中读取 IEEE 754 单精度浮点数。
			* @return	一个 IEEE 754 单精度浮点数。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			float readFloat() const;

			/**
			* 从文件流、字节流或字节数组中读取带符号的 32 位整数。
			* @return	返回值的范围是从 -2147483648 到 2147483647。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			int  readInt() const;


			/**
			* 使用指定的字符集从文件流、字节流或字节数组中读取指定长度的多字节字符串。
			* @param	length	要从字节流中读取的字节数。
			* @param	charSet	表示用于解释字节的Encoding
			* @return	UTF-8 编码的字符串。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			CFLString readMultiByte(size_t length, text::Encoding charset) const;

			/**
			* 从文件流、字节流或字节数组中读取带符号的 16 位整数。
			* @return	返回值的范围是从 -32768 到 32767。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			short readShort() const;

			/**
			* 从文件流、字节流或字节数组中读取无符号的字节。
			* @return	返回值的范围是从 0 到 255。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			unsigned char readUnsignedByte() const;


			/**
			* 从文件流、字节流或字节数组中读取无符号的 32 位整数。
			* @return	返回值的范围是从 0 到 4294967295。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			unsigned int readUnsignedInt() const;

			/**
			* 从文件流、字节流或字节数组中读取无符号的 16 位整数。
			* @return	返回值的范围是从 0 到 65535。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			unsigned short readUnsignedShort() const;

			/**
			* 从文件流、字节流或字节数组中读取 UTF-8 字符串。假定字符串的前缀是无符号的短整型（以字节表示长度）。
			* @return	由字符的字节表示形式生成的 UTF-8 字符串。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			CFLString readUTF() const;

			/**
			* 从字节流或字节数组中读取 UTF-8 字节序列，并返回一个字符串。
			* @param	length	要读取的字节数。
			* @return	由指定长度字符的字节表示形式生成的 UTF-8 字符串。
			* @throws	EOFException 没有足够的数据可供读取。
			*/
			CFLString readUTFBytes(size_t length) const;

		private:
			std::shared_ptr<binaryreader_indata> indata;
			
		};

		


	}
}


#endif // !CFL_BINARYREADER_H
