#ifndef CFL_UCHAR_H
#define CFL_UCHAR_H

namespace cfl
{
	//�����ַ��� Unicode ���
	namespace unicodeCategory
	{
		//�����ַ��� Unicode ���
		enum UnicodeCategory
		{
			UppercaseLetter = 0, //��д��ĸ�� �� Unicode ���롰Lu������ĸ����д����ʾ�� ֵΪ 0��
			LowercaseLetter = 1,//Сд��ĸ�� �� Unicode ���롰Ll������ĸ��Сд����ʾ�� ֵΪ 1��
			TitlecaseLetter =2,//������ĸ��д����ĸ�� �� Unicode ���롰Lt������ĸ��������ĸ��д����ʾ�� ֵΪ 2��
			ModifierLetter =3,//���η���ĸ�ַ������Ƕ���ʽ�ļ���ַ���ָʾǰ����ĸ���޸ġ� �� Unicode ���롰Lm������ĸ�����η�����ʾ�� ֵΪ 3��
			OtherLetter=4,// �����ڴ�д��ĸ��Сд��ĸ��������ĸ��д�����η���ĸ����ĸ�� �� Unicode ���롰Lo������ĸ����������ʾ�� ֵΪ 4��
			NonSpacingMark=5,// ָʾ���ַ����޸ĵķǼ���ַ��� �� Unicode ���롰Mn�������ţ��Ǽ�ࣩ��ʾ�� ֵΪ 5��
			SpacingCombiningMark=6, //����ַ�����ָʾ���ַ����޸Ĳ�Ӱ����ַ��ı�־���ŵĿ�ȡ� �� Unicode ���롰Mc�������ţ������ϣ���ʾ�� ֵΪ 6��
			EnclosingMark=7,// ��շ����ַ����������ַ�ǰ��������ַ����������ַ����������� �� Unicode ���롰Me�������ţ���գ���ʾ�� ֵΪ 7��
			DecimalDigitNumber=8,//ʮ���������ַ�������Χ 0 �� 9 �ڵ��ַ��� �� Unicode ���롰Nd�������֣�ʮ�������֣���ʾ�� ֵΪ 8��
			LetterNumber=9, //����ĸ��ʾ�����֣�������ʮ�������֣����磬�������� 5 ����ĸ��V����ʾ�� ��ָʾ���� Unicode ���롰Nl�������֣���ĸ����ʾ�� ֵΪ 9��
			OtherNumber=10, //�Ȳ���ʮ��������Ҳ������ĸ���ֵ����֣�������� 1 / 2�� ��ָʾ���� Unicode ���롰No�������֣���������ʾ�� ֵΪ 10��
			SpaceSeparator=11,//û�б�־���ŵ������ڿ��ƻ��ʽ�ַ��Ŀհ��ַ��� �� Unicode ���롰Zs�����ָ������հף���ʾ�� ֵΪ 11��
			LineSeparator=12, //���ڷָ��ı����е��ַ��� �� Unicode ���롰Zl�����ָ������У���ʾ�� ֵΪ 12��
			ParagraphSeparator=13, //���ڷָ�������ַ��� �� Unicode ���롰Zp�����ָ��������䣩��ʾ�� ֵΪ 13��
			Control=14,//���ƴ����ַ����� Unicode ֵ�� U + 007F������λ�� U + 0000 �� U + 001F �� U + 0080 �� U + 009F ��Χ�ڡ� �� Unicode ���롰Cc�������������ƣ���ʾ�� ֵΪ 14��
			Format=15,//��ʽ�ַ�����Ӱ���ı����ֻ��ı����������������ͨ��������֡� �� Unicode ���롰Cf������������ʽ����ʾ�� ֵΪ 15��
			Surrogate=16,//�ߴ������ʹ������ַ��� �� �� Unicode ���롰Cs�����������������ʾ�� ֵΪ 16��
			PrivateUse=17, //ר���ַ����� Unicode ֵ�ڷ�Χ U + E000 �� U + F8FF �ڡ� �� Unicode ���롰Co����������ר�ã���ʾ�� ֵΪ 17��
			ConnectorPunctuation=18, //���������ַ������ӷ�����ַ��� �� Unicode ���롰Pc������㣬���ӷ�����ʾ�� ֵΪ 18��
			DashPunctuation=19,//�̻��߻����ַ��ַ��� �� Unicode ���롰Pd������㣬�̻��ߣ���ʾ�� ֵΪ 19��
			OpenPunctuation=20,//�ɶԵı����ţ��������š������źʹ����ţ��Ŀ�ʼ�ַ��� �� Unicode ���롰Ps������㣬��ʼ����ʾ�� ֵΪ 20��
			ClosePunctuation=21, //�ɶԵı����ţ��������š������źʹ����ţ��Ľ����ַ��� �� Unicode ���롰Pe������㣬��������ʾ�� ֵΪ 21��
			InitialQuotePunctuation=22,//�����Ż�ǰ�����ַ��� �� Unicode ���롰Pi������㣬ǰ���ţ���ʾ�� ֵΪ 22��
			FinalQuotePunctuation=23,//�����Ż�������ַ��� �� Unicode ���롰Pf������㣬�����ţ���ʾ�� ֵΪ 23��
			OtherPunctuation=24,//���������ӷ����̻��ߡ���ʼ��㡢������㡢ǰ���Ż�����ŵı���ַ��� �� Unicode ���롰Po������㣬��������ʾ�� ֵΪ 24��
			MathSymbol=25, //��ѧ�����ַ������硰 + ���� = ���� �� Unicode ���롰Sm�������ţ���ѧ����ʾ�� ֵΪ 25��
			CurrencySymbol=26,//���ҷ����ַ��� �� Unicode ���롰Sc�������ţ����ң���ʾ�� ֵΪ 26��
			ModifierSymbol=27,//���η������ַ�����ָʾ�����ַ����޸ġ� ���磬����б�ߺ�ָʾ����������Ϊ���ӣ��Ҳ������Ϊ��ĸ�� ��ָʾ���� Unicode ���롰Sk�������ţ����η�����ʾ�� ֵΪ 27��
			OtherSymbol=28,//��������ѧ���š����ҷ��Ż����η����ŵķ����ַ��� �� Unicode ���롰So�������ţ���������ʾ�� ֵΪ 28��
			OtherNotAssigned=29, //δָ�ɸ��κ� Unicode �����ַ��� �� Unicode ���롰Cn����������δ���䣩��ʾ�� ֵΪ 29��

		};

	}
	//˫������ö��
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
	//�ַ���Ϲ���
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


	//UCS4�����ַ�
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




		//��ȡunicode���
		static unicodeCategory::UnicodeCategory getUnicodeCategory(const UChar c);

		static bool isWhiteSpace(const UChar c);

		static UChar toUpper(const UChar c);

		static UChar toLower(const UChar c);
	};
}


#endif // !CFL_UCHAR_H
