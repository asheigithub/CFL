// FreeTypeWapperLib.h

#pragma once

using namespace System;


#include "FreeTypeWapper.h"



namespace cfl
{
	namespace tools
	{
		namespace freetypewapper
		{
			
			public ref struct FontInfoWapper
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

			public ref struct KerningWapper
			{
				int det_x;
				int det_y;
			};

			public ref struct GlyphWapper
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

				array<unsigned char>^ bitmap;

			};


			public ref class FreeTypeWapperLib
			{
			public:
				FreeTypeWapperLib()
				{
					wapper = new FreeTypeWapper();
				}

				~FreeTypeWapperLib()
				{
					delete wapper;

					if (buff)
					{
						delete[] buff;
						buff = nullptr;
					}

				}


				// TODO:  在此处添加此类的方法。
				bool init(array<unsigned char>^ bytes,
					unsigned int faceidx);

				FontInfoWapper^ getInfo();
				
				System::Collections::Generic::List<System::UInt64>^ getAllCharCodes();

				/// <summary>
				/// 查询字距微调
				/// </summary>
				KerningWapper^ queryKerning(unsigned long leftcode, unsigned long rightcode);

				/// <summary>
				/// 获取字形图像
				/// </summary>
				GlyphWapper^   getGlyph(unsigned short pixelsize, unsigned long charcode);

			private:
				FreeTypeWapper* wapper;
				unsigned char* buff = nullptr;
			};
			

		}
	}
}