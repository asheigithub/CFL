#ifndef CFLTOOL_FREETYPEWAPPER_H
#define CFLTOOL_FREETYPEWAPPER_H
#include "stdafx.h"
#include <memory>

#include <vector>

namespace cfl
{
	namespace tools
	{
		namespace freetypewapper
		{
			struct FontInfo
			{
				bool use_kerning;

				short ascender;

				short descender;

				int bbox_xMax;
				int bbox_xMin;
				int bbox_yMax;
				int bbox_yMin;


				int units_per_EM;
				int height;

			};

			struct kerning
			{
				int det_x;
				int det_y;
			};

			struct Glyph
			{
				int imagewidth;
				int imageheight;

				int advance_x;
				int advance_y;

				int pen_x;
				int pen_y;

				int padleft;
				int padtop;

				int clipwidth;
				int clipheight;

				std::shared_ptr<unsigned char> bitmap;

			};


			struct WapperData;
			
			class FreeTypeWapper
			{
				

				std::shared_ptr<WapperData> data;
			public:

				FreeTypeWapper();
				

				bool initFromByteArray(const unsigned char* bytes, unsigned int filesize, unsigned int faceidx );

				FontInfo getInfo() const;
				
				std::shared_ptr< std::vector<unsigned long>> getAllCharCodes();

				kerning queryKerning(unsigned long leftcode,unsigned long rightcode);

				Glyph   getGlyph(unsigned short pixelsize, unsigned long charcode);


			};




		}

	}
}





#endif // !CFLTOOL_FREETYPEWAPPER_H
