#ifndef CFL_TEXTELEMENTENUMERATOR_H
#define CFL_TEXTELEMENTENUMERATOR_H

#include "../CFLString.h"

namespace cfl
{
	namespace text
	{
		//�ı�Ԫ��
		struct TElement
		{
			friend class TextElementEnumerator;
			
			const unsigned int*  uchars;
			size_t len;
			size_t index;


		private:

			TElement(CFLString str)
				:uchars(nullptr), len(0), str(str), index(0)
			{
			}


			const CFLString str;
		};

		//�ı�Ԫ��ö����
		class TextElementEnumerator
		{
		public:
			TextElementEnumerator(const CFLString string)
				:string(string), element(string), uc( unicodeCategory::UppercaseLetter )
			{
				
			}
			~TextElementEnumerator()=default;

			//��ö��������Ϊ���ʼλ�ã���λ��λ���ַ����е�һ���ı�Ԫ��֮ǰ��
			void reset();

			//��ö����ǰ�Ƶ��ַ�������һ���ı�Ԫ�ء�
			bool moveNext();

			//��ȡ�ַ����еĵ�ǰ�ı�Ԫ�ء�
			TElement* getCurrent()  { return &element; }
		private:
			const CFLString string;
			size_t index;
			TElement element;
			unicodeCategory::UnicodeCategory uc;
		};


	}
}



#endif // !CFL_TEXTELEMENTENUMERATOR_H
