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
				begin,//ָ�����Ŀ�ͷ��
				current,//ָ�����ڵĵ�ǰλ�á�
				end//ָ�����Ľ�β��
			};
		}


		/*
		�ṩ�ֽ����е�һ����ͼ������һ�������ࡣ
		*/
		class stream
		{
		public:
			virtual ~stream()=default;

			//��������������дʱ����ȡ�����ȣ����ֽ�Ϊ��λ����
			virtual size_t getLength() = 0;

			//��������������дʱ����ȡ�����õ�ǰ���е�λ�á�
			virtual int getPosition() const = 0;
			//��������������дʱ����ȡ�����õ�ǰ���е�λ�á�
			virtual void   setPosition(int value) = 0;

			virtual void dispose() = 0;
			/*
			��������������дʱ���ӵ�ǰ����ȡ�ֽ����У����������е�λ��������ȡ���ֽ�����
			buffer
				�ֽ����顣�˷�������ʱ���û���������ָ�����ַ����飬������� offset �� (offset + count -1) ֮���ֵ�ɴӵ�ǰԴ�ж�ȡ���ֽ��滻��
			offset
				buffer �еĴ��㿪ʼ���ֽ�ƫ�������Ӵ˴���ʼ�洢�ӵ�ǰ���ж�ȡ�����ݡ�
			count
				Ҫ�ӵ�ǰ��������ȡ���ֽ�����
			return
				���뻺�����е����ֽ���������ܶ��ֽڵ�ǰ�����ã������ֽ�������С��������ֽ���������ѵ�������β����Ϊ�� (0)��
			*/
			virtual unsigned int read(unsigned char* buffer,int offset,int size )=0;

			//�����ж�ȡһ���ֽڣ��������ڵ�λ����ǰ����һ���ֽڣ���������ѵ�������β���򷵻� -1��
			virtual int readByte()=0;

			/*
			��������������дʱ�����õ�ǰ���е�λ�á�
			offset
				����� origin �������ֽ�ƫ������
			origin
				SeekOrigin ���͵�ֵ��ָʾ���ڻ�ȡ��λ�õĲο��㡣
			����ֵ
				Type: int
				��ǰ���е���λ�á�
			*/
			virtual int seek(int offset, seekOrigin::SeekOrigin origin) = 0;


			


		};


	}
}


#endif // !CFLSTREAM_H
