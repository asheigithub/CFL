#ifndef _CFL_ENCODING_
#define _CFL_ENCODING_

#include "../CFLString.h"
namespace cfl
{
	
	namespace text
	{
		class GBKEncoding;

		class EncodingBase
		{
		public:
			virtual ~EncodingBase()=default;
			 virtual std::shared_ptr<char> getBytes(const CFLString&)=0 ;
			 virtual CFLString getString(const char*)=0 ;


		private:

		};

		class Endoder
		{
		public:
			 //��ȡGBK������
			static EncodingBase* getGBK();
			//��ȡUTF-8������
			static EncodingBase* getUTF8();
			//��ȡUnicode  ������
			static EncodingBase* getUnicode();
		};

	}
}

#endif