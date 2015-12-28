
#include "CFLString.h"
#include "Text/CFLEncoding.h"

#include "headers/CFLConvertUTF.h"

#include <mutex>
#include <atomic>

namespace cfl
{
	using namespace cfl::text;

	struct atomicdata
	{
		atomicdata()
		{
			
		}

		~atomicdata()
		{
			auto at = ansi_chars.load(std::memory_order_relaxed);
			if (at !=nullptr)
			{
				delete at;
				ansi_chars.store(nullptr, std::memory_order_relaxed );
			}
			auto ut = utf8_chars.load(std::memory_order_relaxed);
			if ( ut !=nullptr)
			{
				delete ut;
				utf8_chars.store(nullptr, std::memory_order_relaxed);
			}
			auto uct = ucs4.load(std::memory_order_relaxed);
			if ( uct != nullptr)
			{
				delete uct;
				ucs4.store(nullptr, std::memory_order_relaxed);
			}
		}

		std::mutex mtx;
		std::atomic< char*> ansi_chars;
		std::atomic< char*> utf8_chars;
		std::atomic< unsigned int*> ucs4;

		std::atomic<size_t> hashcode;

	};

	size_t	string_data::get_hashcode() const
	{
		return a_data->hashcode.load(std::memory_order_relaxed);
	}
	void	string_data::set_hashcode(size_t hash)
	{
		a_data->hashcode.store(hash, std::memory_order_relaxed);
	}

	const char* string_data::get_ansi_chars() const
	{
		return a_data->ansi_chars.load( std::memory_order_relaxed );
	}

	const char* string_data::get_utf8_chars() const
	{
		return a_data->utf8_chars.load(std::memory_order_relaxed);
	}

	const unsigned int* string_data::get_ucs4() const
	{
		return a_data->ucs4.load(std::memory_order_relaxed);
	}

	string_data::string_data(const char* chars,const text::Encoding encode)
		:a_data( new atomicdata() )
	{
		switch (encode)
		{
		case Encoding::gbk:
			{
				a_data->hashcode.store(0, std::memory_order_relaxed);

				if (chars != nullptr)
				{
					
					ansi_len = strlen(chars);

					auto temp = new char[ansi_len + 1];
					memcpy(const_cast<char*>(temp), chars, ansi_len + 1);
					a_data->ansi_chars.store( temp ,std::memory_order_relaxed );
					

				}
				else
				{
					a_data->ansi_chars = new char[1]{'\0'};
					ansi_len = 0;
				}
				

				a_data->utf8_chars.store(nullptr, std::memory_order_relaxed);
				utf8_len = 0;

				break;
			}	
		case Encoding::utf8:
			{
				a_data->hashcode.store(0, std::memory_order_relaxed);

				a_data->ansi_chars.store(nullptr, std::memory_order_relaxed);
				ansi_len = 0;

				if (chars != nullptr)
				{
					
					utf8_len = strlen(chars);

					auto temp =new char[utf8_len + 1];
					memcpy(temp, chars, utf8_len + 1);
					a_data->utf8_chars.store(temp, std::memory_order_relaxed);
					
				}
				else
				{
					a_data->utf8_chars = new char[1]{'\0'};
					utf8_len = 0;
				}

				break;
			}
			
		}

		a_data->ucs4.store(nullptr, std::memory_order_relaxed);
		ucs4_len = 0;

	}
	string_data::~string_data()
	{
		if (a_data)
		{
			delete a_data;
			a_data = nullptr;
		}
	}

	void string_data::prepare_ansi_str(const CFLString& str)
	{

		const char* tmp = a_data->ansi_chars.load(std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_acquire);
		if (tmp == nullptr)
		{
			std::unique_lock<std::mutex> lock((a_data->mtx));
			
			tmp = a_data->ansi_chars.load(std::memory_order_relaxed);
			if (tmp == nullptr)
			{
				auto gbkencoder = text::Endoder::getGBK();

				auto chars= gbkencoder->getBytes(str);

				//����Դ��ansi_chars
				ansi_len = strlen( chars.get() );
				tmp = new char[ansi_len + 1];
				memcpy(const_cast<char*>(tmp), chars.get(), ansi_len + 1);

				std::atomic_thread_fence(std::memory_order_release);

				a_data->ansi_chars.store( const_cast<char*>( tmp), std::memory_order_relaxed);
			}
		}
	}
	
	void string_data::prepare_utf8_str(const CFLString& str)
	{

		const char* pt = a_data->utf8_chars.load(std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_acquire);
		if (pt == nullptr)
		{
			std::unique_lock<std::mutex> lock((a_data->mtx));

			pt = a_data->utf8_chars.load(std::memory_order_relaxed);

			if (pt == nullptr)
			{
				auto len = strlen(a_data->ansi_chars);

				char* temp = new char[len * 4 + 1];
				memset( temp, 0, len * 4 + 1);

				utf8_len = GBKTOUTF8(a_data->ansi_chars.load(std::memory_order_relaxed), temp, len);

				std::atomic_thread_fence(std::memory_order_release);

				a_data->utf8_chars.store(temp, std::memory_order_relaxed);
			}
		}
	}

	
	CFLString::CFLString(const char* chars,const Encoding encoding):
		strdata(new string_data(chars,encoding))
	{
		
	}
	
	CFLString::~CFLString()
	{
		
	}

	bool CFLString::equals(const CFLString& rhs) const
	{
		return compare( std::move( *this), std::move( rhs)) == 0;
	}

	int CFLString::compare(const CFLString& lhs, const CFLString& rhs)
	{
		return strcmp(lhs._utf8_str(), rhs._utf8_str());

	}
	CFLString operator+(const CFLString& lhs, const CFLString& rhs)
	{
		const char* c1;
		const char* c2;

		size_t c1l;
		size_t c2l;

		text::Encoding encoding;

		if (lhs.strdata->a_data->ansi_chars.load(std::memory_order_relaxed) != nullptr 
			&& rhs.strdata->a_data->ansi_chars.load(std::memory_order_relaxed) != nullptr)
		{
			encoding = Encoding::gbk;

			c1 = lhs.strdata->a_data->ansi_chars.load(std::memory_order_relaxed);
			c2 = rhs.strdata->a_data->ansi_chars.load(std::memory_order_relaxed);

			c1l = lhs.strdata->ansi_len;
			c2l = rhs.strdata->ansi_len;
		}
		else if (lhs.strdata->a_data->utf8_chars.load(std::memory_order_relaxed) != nullptr 
			&& rhs.strdata->a_data->utf8_chars.load(std::memory_order_relaxed) != nullptr)
		{
			encoding = Encoding::utf8;

			c1 = lhs.strdata->a_data->utf8_chars.load(std::memory_order_relaxed);
			c2 = rhs.strdata->a_data->utf8_chars.load(std::memory_order_relaxed);

			c1l = lhs.strdata->utf8_len;
			c2l = rhs.strdata->utf8_len;
		}
		else
		{
			encoding = Encoding::utf8;

			c1 = lhs._utf8_str();
			c2 = rhs._utf8_str();

			c1l = lhs.strdata->utf8_len;
			c2l = rhs.strdata->utf8_len;
		}

		char* copy = new char[c1l + c2l + 1];

		memcpy(copy, c1, c1l);
		memcpy(copy + c1l, c2, c2l + 1);

		CFLString ret = CFLString(copy, encoding);

		delete copy;

		return ret;
	}

	CFLString cfl::CFLString::empty;

}