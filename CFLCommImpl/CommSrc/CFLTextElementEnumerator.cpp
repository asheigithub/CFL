#include "Text/CFLTextElementEnumerator.h"

namespace cfl
{
	namespace text
	{
		void TextElementEnumerator::reset()
		{
			string.init_ucs4();
			index = 0;

			if (index < string.strdata->ucs4_len) {
				// If we have more than 1 character, get the category of the current char.
				uc = UChar::getUnicodeCategory(string.strdata->get_ucs4()[index]); //CharUnicodeInfo.InternalGetUnicodeCategory(str, index, out charLen);
			}
		}

		static bool IsCombiningCategory(unicodeCategory::UnicodeCategory uc) {

			return (
				uc == unicodeCategory::UnicodeCategory::NonSpacingMark ||
				uc == unicodeCategory::UnicodeCategory::SpacingCombiningMark ||
				uc == unicodeCategory::UnicodeCategory::EnclosingMark
				);
		}

		static int GetCurrentTextElementLen(const unsigned int* str, int index, int len,  unicodeCategory::UnicodeCategory& ucCurrent,  int currentCharCount)
		{
			//Contract.Assert(index >= 0 && len >= 0, "StringInfo.GetCurrentTextElementLen() : index = " + index + ", len = " + len);
			//Contract.Assert(index < len, "StringInfo.GetCurrentTextElementLen() : index = " + index + ", len = " + len);
			if (index + currentCharCount == len)
			{
				// This is the last character/surrogate in the string.
				return (currentCharCount);
			}

			// Call an internal GetUnicodeCategory, which will tell us both the unicode category, and also tell us if it is a surrogate pair or not.
			
			auto ucNext = UChar::getUnicodeCategory(str[ index + currentCharCount] );
			if (IsCombiningCategory(ucNext)) {
				// The next element is a combining class.
				// Check if the current text element to see if it is a valid base category (i.e. it should not be a combining category,
				// not a format character, and not a control character).

				if (IsCombiningCategory(ucCurrent)
					|| (ucCurrent == unicodeCategory::UnicodeCategory::Format)
					|| (ucCurrent == unicodeCategory::UnicodeCategory::Control)
					|| (ucCurrent == unicodeCategory::UnicodeCategory::OtherNotAssigned)
					|| (ucCurrent == unicodeCategory::UnicodeCategory::Surrogate))    // An unpair high surrogate or low surrogate
				{
					// Will fall thru and return the currentCharCount
				}
				else {
					int startIndex = index; // Remember the current index.

					// We have a valid base characters, and we have a character (or surrogate) that is combining.
					// Check if there are more combining characters to follow.
					// Check if the next character is a nonspacing character.
					index += currentCharCount + 1;

					while (index < len)
					{
						ucNext = UChar::getUnicodeCategory(str[index]);
						if (!IsCombiningCategory(ucNext)) {
							ucCurrent = ucNext;
							currentCharCount = 1;
							break;
						}
						index += 1;
					}
					return (index - startIndex);
				}
			}
			// The return value will be the currentCharCount.
			int ret = currentCharCount;
			ucCurrent = ucNext;
			// Update currentCharCount.
			currentCharCount = 1;
			return (ret);
		}



		bool TextElementEnumerator::moveNext()
		{
			if (index >= string.strdata->ucs4_len)
			{
				// Make the index to be greater than strLen so that we can throw exception if GetTextElement() is called.
				index = string.strdata->ucs4_len + 1;
				return (false);
			}

			auto currTextElementLen = GetCurrentTextElementLen(
				string.strdata->get_ucs4(), index, string.strdata->ucs4_len,uc,1);

			element.index = index;
			element.uchars = string.strdata->get_ucs4();
			element.len = currTextElementLen;

			index += currTextElementLen;
			return (true);


		}

	}
}