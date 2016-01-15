#ifndef CFL_FONTTEXTURE_H
#define CFL_FONTTEXTURE_H

#include "Graphic/CFLGameImage.h"
#include "Content/CFLMemoryDataSource.h"
#include <memory>

#include "Content/CFLHuffman.h"

namespace cfl
{
	namespace graphic
	{
		namespace font
		{
			const size_t FONT_TEXTURESIZE=512;

			struct AddGlyphResut
			{
				AddGlyphResut(std::shared_ptr<Texture2DRef> texRef,
					size_t intex_x,
					size_t intex_y)
					:
					texRef(texRef),
					intex_x(intex_x),
					intex_y(intex_y)
				{
					
				}



				std::shared_ptr<Texture2DRef> texRef;
				size_t intex_x;
				size_t intex_y;

			};


			void updateFontTexture();

			AddGlyphResut addGlyph( cfl::file::DirType fontdir,CFLString fontpath  ,
				size_t imagedataoffset, size_t datasize , bool isfirstglyph,
				std::shared_ptr<cfl::content::huffman::DecodeInfo> decodeinfo ,double q,double d);

			void clearFontTexture();

			

			void unDCT(unsigned char* result, char* zeroandgroup, 
				unsigned int zeroandgrouplen, char* vlicode, 
				unsigned int vlicodelen , double q,double d );
			


		}


	}
}


#endif // !CFL_FONTTEXTURE_H
