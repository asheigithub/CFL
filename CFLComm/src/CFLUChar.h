#ifndef CFL_UCHAR_H
#define CFL_UCHAR_H

namespace cfl
{
	//定义字符的 Unicode 类别。
	namespace unicodeCategory
	{
		//定义字符的 Unicode 类别。
		enum UnicodeCategory
		{
			UppercaseLetter = 0, //大写字母。 由 Unicode 代码“Lu”（字母，大写）表示。 值为 0。
			LowercaseLetter = 1,//小写字母。 由 Unicode 代码“Ll”（字母，小写）表示。 值为 1。
			TitlecaseLetter =2,//词首字母大写的字母。 由 Unicode 代码“Lt”（字母，词首字母大写）表示。 值为 2。
			ModifierLetter =3,//修饰符字母字符，它是独立式的间距字符，指示前面字母的修改。 由 Unicode 代码“Lm”（字母，修饰符）表示。 值为 3。
			OtherLetter=4,// 不属于大写字母、小写字母、词首字母大写或修饰符字母的字母。 由 Unicode 代码“Lo”（字母，其他）表示。 值为 4。
			NonSpacingMark=5,// 指示基字符的修改的非间距字符。 由 Unicode 代码“Mn”（符号，非间距）表示。 值为 5。
			SpacingCombiningMark=6, //间距字符，它指示基字符的修改并影响基字符的标志符号的宽度。 由 Unicode 代码“Mc”（符号，间距组合）表示。 值为 6。
			EnclosingMark=7,// 封闭符号字符，它将基字符前面的所有字符（包括基字符）括起来。 由 Unicode 代码“Me”（符号，封闭）表示。 值为 7。
			DecimalDigitNumber=8,//十进制数字字符，即范围 0 到 9 内的字符。 由 Unicode 代码“Nd”（数字，十进制数字）表示。 值为 8。
			LetterNumber=9, //由字母表示的数字，而不是十进制数字，例如，罗马数字 5 由字母“V”表示。 此指示符由 Unicode 代码“Nl”（数字，字母）表示。 值为 9。
			OtherNumber=10, //既不是十进制数字也不是字母数字的数字，例如分数 1 / 2。 此指示符由 Unicode 代码“No”（数字，其他）表示。 值为 10。
			SpaceSeparator=11,//没有标志符号但不属于控制或格式字符的空白字符。 由 Unicode 代码“Zs”（分隔符，空白）表示。 值为 11。
			LineSeparator=12, //用于分隔文本各行的字符。 由 Unicode 代码“Zl”（分隔符，行）表示。 值为 12。
			ParagraphSeparator=13, //用于分隔段落的字符。 由 Unicode 代码“Zp”（分隔符，段落）表示。 值为 13。
			Control=14,//控制代码字符，其 Unicode 值是 U + 007F，或者位于 U + 0000 到 U + 001F 或 U + 0080 到 U + 009F 范围内。 由 Unicode 代码“Cc”（其他，控制）表示。 值为 14。
			Format=15,//格式字符，它影响文本布局或文本处理操作，但是它通常不会呈现。 由 Unicode 代码“Cf”（其他，格式）表示。 值为 15。
			Surrogate=16,//高代理项或低代理项字符。 。 由 Unicode 代码“Cs”（其他，代理项）表示。 值为 16。
			PrivateUse=17, //专用字符，其 Unicode 值在范围 U + E000 到 U + F8FF 内。 由 Unicode 代码“Co”（其他，专用）表示。 值为 17。
			ConnectorPunctuation=18, //连接两个字符的连接符标点字符。 由 Unicode 代码“Pc”（标点，连接符）表示。 值为 18。
			DashPunctuation=19,//短划线或连字符字符。 由 Unicode 代码“Pd”（标点，短划线）表示。 值为 19。
			OpenPunctuation=20,//成对的标点符号（例如括号、方括号和大括号）的开始字符。 由 Unicode 代码“Ps”（标点，开始）表示。 值为 20。
			ClosePunctuation=21, //成对的标点符号（例如括号、方括号和大括号）的结束字符。 由 Unicode 代码“Pe”（标点，结束）表示。 值为 21。
			InitialQuotePunctuation=22,//左引号或前引号字符。 由 Unicode 代码“Pi”（标点，前引号）表示。 值为 22。
			FinalQuotePunctuation=23,//右引号或后引号字符。 由 Unicode 代码“Pf”（标点，后引号）表示。 值为 23。
			OtherPunctuation=24,//不属于连接符、短划线、开始标点、结束标点、前引号或后引号的标点字符。 由 Unicode 代码“Po”（标点，其他）表示。 值为 24。
			MathSymbol=25, //数学符号字符，例如“ + ”或“ = ”。 由 Unicode 代码“Sm”（符号，数学）表示。 值为 25。
			CurrencySymbol=26,//货币符号字符。 由 Unicode 代码“Sc”（符号，货币）表示。 值为 26。
			ModifierSymbol=27,//修饰符符号字符，它指示环绕字符的修改。 例如，分数斜线号指示其左侧的数字为分子，右侧的数字为分母。 此指示符由 Unicode 代码“Sk”（符号，修饰符）表示。 值为 27。
			OtherSymbol=28,//不属于数学符号、货币符号或修饰符符号的符号字符。 由 Unicode 代码“So”（符号，其他）表示。 值为 28。
			OtherNotAssigned=29, //未指派给任何 Unicode 类别的字符。 由 Unicode 代码“Cn”（其他，未分配）表示。 值为 29。

		};

	}
	//双向类型枚举
	namespace bidiClass
	{
		enum BidiClass
		{
			L,//Left - to - Right
			LRE, //Left - to - Right Embedding
			LRO, //Left - to - Right Override
			R, //Right - to - Left
			AL ,//Right - to - Left Arabic
			RLE,//Right - to - Left Embedding
			RLO,//Right - to - Left Override
			PDF,//Pop Directional Format
			EN,//European Number
			ES,//European Number Separator
			ET,//European Number Terminator
			AN,//Arabic Number
			CS,//Common Number Separator
			NSM,//Non - Spacing Mark
			BN,//Boundary Neutral
			B,//Paragraph Separator
			S,//Segment Separator
			WS,//Whitespace
			ON,//Other Neutrals
		};
	}
	//字符结合规则
	namespace canonicalCombiningClass
	{
		enum CanonicalCombiningClass
		{
			c0, //Spacing, split, enclosing, reordrant, and Tibetan subjoined
			c1, // Overlays and interior
			c7, // Nuktas
			c8, // Hiragana / Katakana voicing marks
			c9, // Viramas
			c10, // Start of fixed position classes

			c199, // End of fixed position classes
			c200, // Below left attached
			c202, // Below attached
			c204, // Below right attached
			c208, // Left attached(reordrant around single base character)
			c210, // Right attached
			c212, // Above left attached
			c214, // Above attached
			c216, // Above right attached
			c218, // Below left
			c220, // Below
			c222, // Below right
			c224, // Left(reordrant around single base character)
			c226, // Right
			c228, // Above left
			c230, // Above
			c232, // Above right
			c233, // Double below
			c234, // Double above
			c240, // Below(iota subscript)

		};
	}


	//UCS4编码字符
	struct UChar 
	{
		UChar():charCode(0){}
		UChar(unsigned int code) :charCode(code){}

		unsigned int charCode;

		bool operator==(const UChar& rhs) const
		{
			return charCode == rhs.charCode;
		}

		bool operator!=(const UChar& rhs) const
		{
			return charCode != rhs.charCode;
		}




		//获取unicode类别
		static unicodeCategory::UnicodeCategory getUnicodeCategory(const UChar c);

		static bool isWhiteSpace(const UChar c);

		static UChar toUpper(const UChar c);

		static UChar toLower(const UChar c);
	};
}


#endif // !CFL_UCHAR_H
