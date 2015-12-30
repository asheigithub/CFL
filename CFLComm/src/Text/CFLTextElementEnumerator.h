#ifndef CFL_TEXTELEMENTENUMERATOR_H
#define CFL_TEXTELEMENTENUMERATOR_H

#include "../CFLString.h"

namespace cfl
{
	namespace text
	{
		//文本元素
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

		//文本元素枚举器
		class TextElementEnumerator
		{
		public:
			TextElementEnumerator(const CFLString string)
				:string(string), element(string), uc( unicodeCategory::UppercaseLetter )
			{
				
			}
			~TextElementEnumerator()=default;

			//将枚举数设置为其初始位置，该位置位于字符串中第一个文本元素之前。
			void reset();

			//将枚举数前移到字符串的下一个文本元素。
			bool moveNext();

			//获取字符串中的当前文本元素。
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
