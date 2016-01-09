#ifndef _CFL_STRING_
#define _CFL_STRING_

#include "CFLUChar.h"
#include "Text/CFLEncodingEnum.h"
#include <memory>
#include <unordered_map>

namespace cfl
{
	namespace text
	{
		class GBKEncoding;
		class TextElementEnumerator;
	}



	class CFLString;

	struct atomicdata;

	class string_data
	{
	public:
		
		string_data(const UChar uchar);
		string_data(const unsigned int* ucs4);
		string_data(const char* chars,const text::Encoding encode );
		~string_data();

		void prepare_ansi_str(const CFLString&);
		void prepare_utf8_str(const CFLString&);
		void prepare_ucs4(const CFLString&);


		atomicdata* a_data;

		const char* get_ansi_chars() const;
		size_t ansi_len;

		const char* get_utf8_chars() const;
		size_t utf8_len;

		const unsigned int* get_ucs4() const;
		size_t ucs4_len;

		size_t	get_hashcode() const;
		void	set_hashcode(size_t hash);
		
	};



	//�ַ���
	class CFLString
	{
		friend class string_data;
		friend class text::TextElementEnumerator;
		friend class text::GBKEncoding;
		friend CFLString operator+(const CFLString& lhs, const CFLString& rhs);
	public:

		static CFLString empty;
		
		CFLString() :CFLString(""){}

		CFLString(const UChar uchar);
		CFLString(const unsigned int* ucs4);
		CFLString(const char* chars) : CFLString(chars, text::Encoding::gbk){}
		CFLString(const char*, const text::Encoding);

		~CFLString();

		inline bool operator==(const CFLString& rhs) const { return equals(rhs); }
		inline bool operator!=(const CFLString& rhs) const { return !equals(rhs); }
		inline CFLString& operator +=(const CFLString& rhs) { 
			strdata = (*this + rhs).strdata;
			return *this;
		}


		bool equals(const CFLString& rhs) const;
		
		//��ȡC����ַ���������Ϊ���л����ı���  win����gbk(ansi),android��utf-8��
		inline const char* c_str() const
		{ 
#ifdef _WIN32
			strdata->prepare_ansi_str(*this);
			return strdata->get_ansi_chars();
#else
			strdata->prepare_utf8_str(*this);
			return strdata->get_utf8_chars();
#endif
		};

		//��ȡC����ַ������������log�����win��android����gbk(ansi)��
		inline const char* log_str() const
		{
			strdata->prepare_ansi_str(*this);
			return strdata->get_ansi_chars();
		};

		


		inline size_t getHashCode() const;
		


		//��ȡ�ַ����ĳ��ȡ�ע�⣬���ô˷�����ִ��ucs4���롣���ص���ucs4�������ַ�������
		size_t length() const;

		//��ȡָ��λ���ַ���ucs4(unicode32)����
		unsigned int charCodeAt(size_t index) const;






	private:

		//��ȡutf8�ַ���
		inline const char* _utf8_str() const
		{
			strdata->prepare_utf8_str(*this);
			return strdata->get_utf8_chars();
		}

		//��ʼ��UCS4�ַ���
		inline void init_ucs4() const
		{
			strdata->prepare_ucs4(*this);
		}

		static int compare(const CFLString& lhs, const CFLString& rhs);

		std::shared_ptr<string_data> strdata;


	};
	CFLString operator+(const CFLString& lhs, const CFLString& rhs);
	
	inline size_t CFLString::getHashCode() const
	{
		auto t_hash = strdata->get_hashcode();
		if (t_hash > 0)
		{
			return t_hash;
		}
		else
		{
			register size_t hash = 0;

			strdata->prepare_utf8_str(*this);

			if (strdata->utf8_len > 0)
			{
				auto str = strdata->get_utf8_chars();
				while (size_t ch = (size_t)*str++)
				{
					hash = hash * 131 + ch;   // Ҳ���Գ���31��131��1313��13131��131313..  

				}
			}

			strdata->set_hashcode(hash);

			return hash;
		}
	};

	struct string_hasher
	{
		size_t operator()(const cfl::CFLString& str) const
		{
			return str.getHashCode();
		}
	};
	

	

}



#endif // !_CFL_STRING_
