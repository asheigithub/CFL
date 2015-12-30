// 这是主 DLL 文件。

#include "stdafx.h"

#include "FreeTypeWapperLib.h"

namespace cfl
{
	namespace tools
	{
		namespace freetypewapper
		{

			FontInfoWapper^ FreeTypeWapperLib::getInfo()
			{
				auto info= wapper->getInfo();

				FontInfoWapper^ ret = gcnew FontInfoWapper();


				
				ret->use_kerning = info.use_kerning;

				ret->ascender = info.ascender;

				ret->descender = info.descender;

				ret->bbox_xMax = info.bbox_xMax;
				ret->bbox_xMin = info.bbox_xMin;
				ret->bbox_yMax = info.bbox_yMax;
				ret->bbox_yMin = info.bbox_yMin;

				ret->units_per_EM = info.units_per_EM;
				ret->height = info.height;

				return ret;
			}

			bool FreeTypeWapperLib::init(array<unsigned char>^ bytes,
				unsigned int faceidx
				)
			{
				if (buff)
				{
					delete[] buff;
					buff = nullptr;
				}

				buff = new unsigned char[bytes->Length];

				auto ptr=System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(bytes, 0);

				memcpy(buff, ptr.ToPointer(), bytes->Length);


				auto ret = wapper->initFromByteArray(buff, bytes->Length, faceidx);

				return ret;
			}

			System::Collections::Generic::List<System::UInt64>^ FreeTypeWapperLib::getAllCharCodes()
			{
				
				auto cs = wapper->getAllCharCodes();
				System::Collections::Generic::List<System::UInt64>^
					codes = gcnew System::Collections::Generic::List<System::UInt64>();

				for (size_t i = 0; i < cs->size(); i++)
				{
					codes->Add( (*cs)[i]);
				}

				return codes;
			}

			KerningWapper^ FreeTypeWapperLib::queryKerning(unsigned long leftcode, unsigned long rightcode)
			{
				KerningWapper^ w = gcnew KerningWapper();

				auto k = wapper->queryKerning(leftcode, rightcode);

				w->det_x = k.det_x;
				w->det_y = k.det_y;

				return w;

			}

			GlyphWapper^   FreeTypeWapperLib::getGlyph(unsigned short pixelsize, unsigned long charcode)
			{
				GlyphWapper^ ret = gcnew GlyphWapper();

				auto fg = wapper->getGlyph(pixelsize, charcode);

				ret->advance_x = fg.advance_x;
				ret->advance_y = fg.advance_y;
				ret->imagewidth = fg.imagewidth;
				ret->imageheight = fg.imageheight;
				ret->padleft = fg.padleft;
				ret->padtop = fg.padtop;
				ret->pen_x = fg.pen_x;
				ret->pen_y = fg.pen_y;

				ret->clipwidth = fg.clipwidth;
				ret->clipheight = fg.clipheight;

				if (fg.clipwidth == 0 || fg.clipheight == 0 || fg.pen_x <-0xffffff || fg.pen_x >0xffffff || fg.pen_y <-0xffffff || fg.pen_y >0xffffff)
				{
					ret->pen_y = 0;
					ret->pen_x = 0;
					ret->padleft = 0;
					ret->padtop = 0;
				}


				ret->bitmap = gcnew array<unsigned char>(ret->imagewidth*ret->imageheight);

				auto p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(ret->bitmap, 0);

				memcpy(p.ToPointer(), fg.bitmap.get(), ret->imagewidth*ret->imageheight);

				return ret;

			}

		}
	}
}