
#include "includes/FreeTypeWapper.h"


#include <ft2build.h>
#include FT_FREETYPE_H 
#include <freetype/ftglyph.h>



namespace cfl
{
	namespace tools
	{
		namespace freetypewapper
		{

			struct WapperData
			{
				FontInfo info;

				FT_Library library; /* 库的句柄 */
				FT_Face face; /* face对象的句柄 */

				~WapperData()
				{
					FT_Done_Face(face);
					FT_Done_FreeType(library);
				}
			};

			FreeTypeWapper::FreeTypeWapper() :data(new WapperData())
			{
				
			}


			bool FreeTypeWapper::initFromByteArray(const unsigned char* bytes, unsigned int filesize, unsigned int faceidx)
			{

				
				auto error = FT_Init_FreeType(&(data->library));
				if (error) { return false; }


				error = FT_New_Memory_Face((data->library),
					bytes, filesize,faceidx, &(data->face)
					);
				if (error == FT_Err_Unknown_File_Format)
				{
					//std::cout << " 可以打开和读这个文件，但不支持它的字体格式" << std::endl;
					return false;
				}
				else if (error)
				{
					//std::cout << " 其它的错误码意味着这个字体文件不能打开和读，或者简单的说它损坏了..." << std::endl;
					return false;
				}
				


				auto use_kerning = FT_HAS_KERNING(data->face);

				data->info.use_kerning = (use_kerning != 0);
				data->info.ascender = data->face->ascender  ;
				data->info.descender = data->face->descender ;
				data->info.bbox_xMax = data->face->bbox.xMax ;
				data->info.bbox_xMin = data->face->bbox.xMin ;
				data->info.bbox_yMax = data->face->bbox.yMax ;
				data->info.bbox_yMin = data->face->bbox.yMin ;
				data->info.height = data->face->height ;
				data->info.units_per_EM = data->face->units_per_EM;





				return true;
			}

			FontInfo FreeTypeWapper::getInfo() const
			{
				return data->info;
			}

			std::shared_ptr< std::vector<unsigned long>> FreeTypeWapper::getAllCharCodes()
			{
				std::vector<unsigned long> ret;

				

				FT_ULong  charcode;
				FT_UInt   gindex;

				
				FT_ULong previous_charcode = 0;
				FT_UInt  previous_glyph = 0;



				charcode = FT_Get_First_Char(data->face, &gindex);
				while (gindex != 0)
				{
					ret.push_back(charcode);
					//枚举所有的charcode
					charcode = FT_Get_Next_Char(data->face, charcode, &gindex);

				}

				return std::shared_ptr< std::vector<unsigned long>>(new std::vector<unsigned long>(ret));

			}

			kerning FreeTypeWapper::queryKerning(unsigned long leftcode, unsigned long rightcode)
			{
				auto g1 = FT_Get_Char_Index(data->face, leftcode);  //0x0E39
				auto g2 = FT_Get_Char_Index(data->face, rightcode);  //0x0E39

				FT_Vector delta;

				FT_Get_Kerning(data->face, g1, g2,
					FT_KERNING_DEFAULT, &delta);

				kerning k;
				k.det_x = delta.x;
				k.det_y = delta.y;


				return k;
			}

			Glyph   FreeTypeWapper::getGlyph(unsigned short pixelsize, unsigned long charcode)
			{
				Glyph ret;
				memset(&ret, 0, sizeof(ret));


				auto error = FT_Set_Pixel_Sizes(
					data->face, /* face对象句柄 */
					0, /* 象素宽度 */
					pixelsize); /* 象素高度 */

				if (error)
				{
					return ret;
				}

				auto glyph_index = FT_Get_Char_Index(data->face, charcode);  //0x0E39

				error = FT_Load_Glyph(
					data->face, /* face对象的句柄 */
					glyph_index, /* 字形索引 */
					FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP); /* 装载标志，参考下面 */

				error = FT_Render_Glyph(data->face->glyph, /* 字形槽 */
					FT_RENDER_MODE_NORMAL); /* 渲染模式 */

				if (error)
				{
					return ret;
				}

				FT_GlyphSlot slot;
				slot = data->face->glyph;

				ret.advance_x = slot->advance.x ;
				ret.advance_y = slot->advance.y ;

				auto bitmap = slot->bitmap;
				auto buffer = bitmap.buffer;

				int imageWidth = pixelsize;// slot->advance.x / 64;//要生成的图片的大小
				int imageHeight = pixelsize;  //face->size->metrics.height / 64;

				int detx = (imageWidth - bitmap.width) / 2;// -slot->bitmap_left;
				int dety = (imageHeight - bitmap.rows) / 2;// -slot->bitmap_top;

				int fontcenterx = detx - slot->bitmap_left;
				int fontcentery = dety + slot->bitmap_top;

				ret.imagewidth = imageWidth;
				ret.imageheight = imageHeight;
				ret.padleft = detx;
				ret.padtop = dety;
				ret.pen_x = fontcenterx;
				ret.pen_y = fontcentery;
				ret.clipwidth = bitmap.width;
				ret.clipheight = bitmap.rows;

				unsigned char* image = new unsigned char[imageWidth * imageHeight];

				ret.bitmap.reset(image,std::default_delete<unsigned char[]>());

				for (int j = 0; j < imageHeight; j++)
				{
					for (int i = 0; i < imageWidth; i++)
					{
						auto c = i - detx;
						auto r = j - dety;

						if (c >= 0 && r >= 0 && c < static_cast<int>( bitmap.width) && r < static_cast<int>( bitmap.rows))
						{
							auto cc = buffer[r * bitmap.width + c];
							//image.SetPixel(i, j, Color(cc, cc, cc));

							image[j*imageHeight + i] = cc;

						}
						else
						{
							image[j*imageHeight + i] = 0;
							//image.SetPixel(i, j, Color(0, 0, 0));
						}

					}
				}
				return ret;

			}

		}
	}
}