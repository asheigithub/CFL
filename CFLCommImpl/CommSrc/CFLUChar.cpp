#include "CFLUChar.h"
#include <unordered_map>
#include "headers/UnicodeTable.h"

namespace cfl
{

	

	static const unsigned int* getinfo(unsigned int code)
	{
		if (code<= 65535)
		{
			return codetable[code];
		}
		else
		{
			auto f = map2.find(code);
			if (f != map2.end())
			{
				return (f->second);
			}
			else
			{
				return (blank);
			}
		}
	}

	unicodeCategory::UnicodeCategory UChar::getUnicodeCategory(const UChar c)
	{
		return static_cast<unicodeCategory::UnicodeCategory>( getinfo(c.charCode)[0]);
	}
	
	bool UChar::isWhiteSpace(const UChar c)
	{
		auto uc = getUnicodeCategory(c);
		// In Unicode 3.0, U+2028 is the only character which is under the category "LineSeparator".
		// And U+2029 is th eonly character which is under the category "ParagraphSeparator".
		switch (uc) 
		{
			case (unicodeCategory::UnicodeCategory::SpaceSeparator) :
			case (unicodeCategory::UnicodeCategory::LineSeparator) :
			case (unicodeCategory::UnicodeCategory::ParagraphSeparator) :
					return (true);
		}

		return (false);

	}

	UChar UChar::toUpper(const UChar c)
	{
		auto info = getinfo(c.charCode);
		if (info[3] != 0)
		{
			return info[3];
		}
		else
		{
			return c;
		}
		
	}

	UChar UChar::toLower(const UChar c)
	{
		auto info = getinfo(c.charCode);
		if (info[4] != 0)
		{
			return info[4];
		}
		else
		{
			return c;
		}
	}


}
