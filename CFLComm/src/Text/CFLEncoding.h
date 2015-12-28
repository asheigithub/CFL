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
			 //获取GBK编码器
			static EncodingBase* getGBK();
			//获取UTF-8编码器
			static EncodingBase* getUTF8();
			//获取Unicode  编码器
			static EncodingBase* getUnicode();
		};

	}
}

#endif