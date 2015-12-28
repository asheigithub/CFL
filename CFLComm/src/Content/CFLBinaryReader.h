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
				"��ȡλ��Խ�硣")
			{
				//LOGE("%s\n",exception::what());
			}

		};

		
		struct binaryreader_indata;
		
		/*
		�����ƶ�ȡ��
		*/
		class BinaryReader
		{
		public:
			BinaryReader(const char* bytes,size_t stpos,size_t length,const Endian endian=Endian::big_endian);
			~BinaryReader();
			
			Endian getEndian() const;
			void   setEndian(const Endian value);


			/**
			* ���ָ��ĵ�ǰλ�ã����ֽ�Ϊ��λ��
			*/
			 size_t getPosition() const;
			 
			 /**
			 * ���ļ�ָ��ĵ�ǰλ�ã����ֽ�Ϊ��λ���ƶ���ָ��λ�á���һ�ε��ö�ȡ����ʱ���ڴ�λ�ÿ�ʼ��ȡ.
			 */
			 void   setPosition( size_t offset);



			/**
			* ���ļ������ֽ������ֽ������ж�ȡ����ֵ����ȡ�����ֽڣ�����ֽڷ��㣬�򷵻� true�����򷵻� false��
			* @return	һ������ֵ������ֽڲ�Ϊ�㣬��Ϊ true������Ϊ false��
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			bool  readBoolean() const;

			/**
			* ���ļ������ֽ������ֽ������ж�ȡ�����ŵ��ֽڡ�
			* @return	����ֵ�ķ�Χ�Ǵ� -128 �� 127��
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			char readByte() const;

			/*
			�����ж�ȡָ�������ֽڣ���䵽Ŀ���ֽ���
			*/
			void readBytes(char* dst,size_t len  ) const;


			/**
			* ���ļ������ֽ������ֽ������ж�ȡ IEEE 754 ˫���ȸ�������
			* @return	һ�� IEEE 754 ˫���ȸ�������
			* @throws	EOFError û���㹻�����ݿɹ���ȡ��
			*/
			double readDouble() const;

			/**
			* ���ļ������ֽ������ֽ������ж�ȡ IEEE 754 �����ȸ�������
			* @return	һ�� IEEE 754 �����ȸ�������
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			float readFloat() const;

			/**
			* ���ļ������ֽ������ֽ������ж�ȡ�����ŵ� 32 λ������
			* @return	����ֵ�ķ�Χ�Ǵ� -2147483648 �� 2147483647��
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			int  readInt() const;


			/**
			* ʹ��ָ�����ַ������ļ������ֽ������ֽ������ж�ȡָ�����ȵĶ��ֽ��ַ�����
			* @param	length	Ҫ���ֽ����ж�ȡ���ֽ�����
			* @param	charSet	��ʾ���ڽ����ֽڵ�Encoding
			* @return	UTF-8 ������ַ�����
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			CFLString readMultiByte(size_t length, text::Encoding charset) const;

			/**
			* ���ļ������ֽ������ֽ������ж�ȡ�����ŵ� 16 λ������
			* @return	����ֵ�ķ�Χ�Ǵ� -32768 �� 32767��
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			short readShort() const;

			/**
			* ���ļ������ֽ������ֽ������ж�ȡ�޷��ŵ��ֽڡ�
			* @return	����ֵ�ķ�Χ�Ǵ� 0 �� 255��
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			unsigned char readUnsignedByte() const;


			/**
			* ���ļ������ֽ������ֽ������ж�ȡ�޷��ŵ� 32 λ������
			* @return	����ֵ�ķ�Χ�Ǵ� 0 �� 4294967295��
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			unsigned int readUnsignedInt() const;

			/**
			* ���ļ������ֽ������ֽ������ж�ȡ�޷��ŵ� 16 λ������
			* @return	����ֵ�ķ�Χ�Ǵ� 0 �� 65535��
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			unsigned short readUnsignedShort() const;

			/**
			* ���ļ������ֽ������ֽ������ж�ȡ UTF-8 �ַ������ٶ��ַ�����ǰ׺���޷��ŵĶ����ͣ����ֽڱ�ʾ���ȣ���
			* @return	���ַ����ֽڱ�ʾ��ʽ���ɵ� UTF-8 �ַ�����
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			CFLString readUTF() const;

			/**
			* ���ֽ������ֽ������ж�ȡ UTF-8 �ֽ����У�������һ���ַ�����
			* @param	length	Ҫ��ȡ���ֽ�����
			* @return	��ָ�������ַ����ֽڱ�ʾ��ʽ���ɵ� UTF-8 �ַ�����
			* @throws	EOFException û���㹻�����ݿɹ���ȡ��
			*/
			CFLString readUTFBytes(size_t length) const;

		private:
			std::shared_ptr<binaryreader_indata> indata;
			
		};

		


	}
}


#endif // !CFL_BINARYREADER_H
