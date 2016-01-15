#ifndef CFL_FONT_H
#define CFL_FONT_H

#include <memory>
#include "../CFLFile.h"
#include "../CFLString.h"

#include "CFLGameImage.h"

#include "../Content/CFLHuffman.h"

namespace cfl
{
	namespace graphic
	{
		namespace font
		{

			//���������Ϣ
			struct FontInfo
			{
				int makeImageSize = 0; //�����ļ�ʱ�õ�ͼ�δ�С���־�������־�ȵ���ֵ�Ķ�Ӧֵ   

				int units_per_EM=0; //��ƴ�С
				short ascender = 0;   //���������ߵľ���
				short descender = 0;  //�ײ������ߵľ���
				int bbox_xMax = 0;    //��������Χ�� ���x
				int bbox_xMin = 0;	  //��������Χ�� ��Сx
				int bbox_yMax = 0;    //��������Χ�� ���y
				int bbox_yMin = 0;    //��������Χ�� ��Сx
				int height = 0;		  //Ĭ���о�
				unsigned int glyphCount = 0;  //��������

				bool useKerning = false;	//�Ƿ�֧���־�΢��

				double q=0;           //ѹ������Q
				double d = 0;		  //ѹ������d

				std::shared_ptr<cfl::content::huffman::DecodeInfo> decodeinfo=nullptr; //������������Ϣ

			};

			//�־�΢��
			struct Kerning
			{
				//unsigned int charcode1 = 0;//����ַ�(����Ӧ��������֮��ƥ�䣬������������charcode���������ö�Ӧ����ֱ����charcode��ѯ)
				//unsigned int charcode2 = 0;//�ұߵ��ַ�

				int detx = 0;
				int dety = 0;

			};

			//����
			struct Glyph
			{
				Glyph()
					:glyphImage(nullptr)
				{

				}

				unsigned int charcode;//�ַ�����
				int advance_x;//�־�X
				int advance_y;//�־�Y
				int padleft;  //ͼ����հ�
				int padtop;   //ͼ���Ͽհ�
				int pen_x;    //ͼ��ԭ��
				int pen_y;    //ͼ��ԭ��
				int clipwidth;   //ͼ��ȥ���հ׺�Ĵ�С
				int clipheight;  //ͼ��ȥ���հ׺�Ĵ�С
				int imagewidth;  //ͼ���С
				int imageheight; //ͼ���С

				

				size_t offset;//ͼ��ƫ����
				size_t bytesize;//ͼ������ռ���ֽ���

				GameImage glyphImage; //���õ�GameImage;

				bool isfirstglyph = false; //�Ƿ��ǵ�һ������(��һ������������˱�����Ϣͷ)
			};


			struct fontInnderData;
			
			class Font
			{
			public:
				~Font()=default;
				//��ȡ������Ϣ
				FontInfo* getInfo() const;
				//��ѯ�־�΢��
				Kerning* queryKerning(unsigned int charcode1,unsigned int charcode2) const;
				
				//��ѯ����
				Glyph* getGlyph(unsigned int charcode) const;
				
				//��ѯ���β�׼��ͼ��
				Glyph* getGlyphForRending(unsigned int charcode) const;
				

				//���ļ��г�ʼ�����塣
				static Font init( cfl::file::DirType dir,CFLString path);

				//��������nameΪkey��������
				static void regFont(const Font font, const CFLString name);

				//��name�����ѱ��������
				static Font getFont(const CFLString name);

				//���������ѱ��������
				static void clearFonts();
			private:
				Font(FontInfo& info);

				std::shared_ptr<fontInnderData> innerData;

			};

		}

	}
}


#endif // !CFL_FONT_H
