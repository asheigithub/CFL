#include "Text/CFLEncoding.h"

#include "headers/GBKTable.h"
#include "headers/CFLConvertUTF.h"

namespace cfl
{

	namespace text
	{
		
		

		class GBKEncoding : public EncodingBase
		{


			std::shared_ptr<char> getBytes(const CFLString& str) override final
			{
				auto utf8char = reinterpret_cast<const unsigned char*>(str._utf8_str());
				auto len = strlen(str._utf8_str());

				unsigned int* ucs4 = new unsigned int[len];
				
				auto ucs4length=UTF8ToUCS4(utf8char, ucs4);

				char* gbkchars = new char[ucs4length * 2 + 1];

				size_t gbksize = 0;

				for (size_t i = 0; i < ucs4length; i++)
				{
					auto ucs4code = ucs4[i];
					if (ucs4code < 0x80)
					{
						memcpy(gbkchars + gbksize, ucs4+i, 1);

						++gbksize;
					}
					else 
					{
						if (ucs4code > 65509) //不在GBK范围内
						{
							ucs4code = 0;
						}

						auto _togbk = ucs4_gbk_table[ucs4code];
						memcpy(gbkchars + gbksize, _togbk, 2);

						gbksize += 2;
					}

				}

				*(gbkchars + gbksize) = '\0';


				//更新源的ucs4
				/*str.strdata->ucs4_len = ucs4length;
				str.strdata->ucs4 = new unsigned int[ucs4length];
				memcpy( const_cast<unsigned int*>(str.strdata->ucs4), ucs4, ucs4length * sizeof(unsigned int));
				*/
				delete ucs4;

				return std::shared_ptr<char>(gbkchars);
			}

			CFLString getString(const  char* bytes) override final
			{
				auto len = strlen(bytes);

				char* temp = new char[len * 4 + 1];
				memset(temp, 0, len * 4 + 1);

				GBKTOUTF8(bytes, temp, len);

				CFLString ret = CFLString(temp,text::Encoding::utf8);

				delete temp;

				return ret;
			}

			

		};


		

		EncodingBase* Endoder::getGBK()
		{
			static GBKEncoding gbk;
			return &gbk;
		}


	}
}