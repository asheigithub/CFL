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

			//字体基本信息
			struct FontInfo
			{
				int makeImageSize = 0; //生成文件时用的图形大小。字距调整和字距等的数值的对应值   

				int units_per_EM=0; //设计大小
				short ascender = 0;   //顶部到基线的距离
				short descender = 0;  //底部到基线的距离
				int bbox_xMax = 0;    //字体最大包围合 最大x
				int bbox_xMin = 0;	  //字体最大包围合 最小x
				int bbox_yMax = 0;    //字体最大包围合 最大y
				int bbox_yMin = 0;    //字体最大包围合 最小x
				int height = 0;		  //默认行距
				unsigned int glyphCount = 0;  //字形数量

				bool useKerning = false;	//是否支持字距微调

				double q=0;           //压缩参数Q
				double d = 0;		  //压缩参数d

				std::shared_ptr<cfl::content::huffman::DecodeInfo> decodeinfo=nullptr; //哈夫曼编码信息

			};

			//字距微调
			struct Kerning
			{
				//unsigned int charcode1 = 0;//左边字符(本来应该是字型之间匹配，但是由于这里charcode和字形正好对应所以直接用charcode查询)
				//unsigned int charcode2 = 0;//右边的字符

				int detx = 0;
				int dety = 0;

			};

			//字形
			struct Glyph
			{
				Glyph()
					:glyphImage(nullptr)
				{

				}

				unsigned int charcode;//字符编码
				int advance_x;//字距X
				int advance_y;//字距Y
				int padleft;  //图像左空白
				int padtop;   //图像上空白
				int pen_x;    //图像原点
				int pen_y;    //图像原点
				int clipwidth;   //图像去掉空白后的大小
				int clipheight;  //图像去掉空白后的大小
				int imagewidth;  //图像大小
				int imageheight; //图像大小

				

				size_t offset;//图形偏移量
				size_t bytesize;//图形数据占用字节数

				GameImage glyphImage; //引用的GameImage;

				bool isfirstglyph = false; //是否是第一个字形(第一个字形里包含了编码信息头)
			};


			struct fontInnderData;
			
			class Font
			{
			public:
				~Font()=default;
				//获取字体信息
				FontInfo* getInfo() const;
				//查询字距微调
				Kerning* queryKerning(unsigned int charcode1,unsigned int charcode2) const;
				
				//查询字形
				Glyph* getGlyph(unsigned int charcode) const;
				
				//查询字形并准备图像
				Glyph* getGlyphForRending(unsigned int charcode) const;
				

				//从文件中初始化字体。
				static Font init( cfl::file::DirType dir,CFLString path);

				//将字体以name为key保存起来
				static void regFont(const Font font, const CFLString name);

				//按name查找已保存的字体
				static Font getFont(const CFLString name);

				//销毁所有已保存的字体
				static void clearFonts();
			private:
				Font(FontInfo& info);

				std::shared_ptr<fontInnderData> innerData;

			};

		}

	}
}


#endif // !CFL_FONT_H
