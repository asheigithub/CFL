// FreeTypeTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//gdi+
#include <comdef.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma  comment(lib, "gdiplus.lib")

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>


#include <ft2build.h>
#include FT_FREETYPE_H 
#include <freetype/ftglyph.h>


#include "huffman.h"
#include <fstream>


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

void MakeImage(FT_Face face,FT_ULong charcode,const wchar_t* fontname);

int _tmain(int argc, _TCHAR* argv[])
{
	//测试huffman

	auto teststr = "cfl::content::unHuffmanCompress(hfile.data.get(), hfile.getTotalFileLength())";

	//cfl::content::test(teststr, strlen(teststr));

	//auto hfile = cfl::content::huffmanCompress(teststr, strlen(teststr));

	//auto un = cfl::content::unHuffmanCompress(hfile.data.get(), hfile.getTotalFileLength());

	//压缩测试
	std::ifstream fstm;
	{
		fstm.open("E:/CFL/Debug/testreadfile/ktx888.ktx", std::ios::binary);
		fstm.seekg(0, std::ios::end);
		auto end_pos = fstm.tellg();

		fstm.seekg(0, std::ios::beg);
		auto size = end_pos.seekpos();

		char* filebuff = new char[size];
		fstm.read(filebuff, size);
		fstm.close();

		auto compressed = cfl::content::huffmanCompress(filebuff, size);
		delete[] filebuff;
	
		std::ofstream fsave;
		fsave.open("E:/CFL/Debug/testreadfile/hufftex.dat", std::ios::binary);
		fsave.write(compressed.data.get(), compressed.getTotalFileLength());
		fsave.close();
	}
	//解压测试
	fstm.open("E:/CFL/Debug/testreadfile/hufftex.dat", std::ios::binary);
	{
		fstm.seekg(0, std::ios::end);
		auto end_pos = fstm.tellg();

		fstm.seekg(0, std::ios::beg);
		auto size = end_pos.seekpos();

		char* filebuff = new char[size];
		fstm.read(filebuff, size);
		fstm.close();

		auto tick = GetTickCount();
		/*for (size_t i = 0; i < 20; i++)
		{*/
		auto uncompress = cfl::content::unHuffmanCompress(filebuff, size);

		//}
		
		auto use = GetTickCount() - tick;
		std::cout << " use tick " << use << std::endl;

		delete[] filebuff;

		std::ofstream fsave;
		fsave.open("E:/CFL/Debug/testreadfile/hufftex.ktx", std::ios::binary);
		fsave.write(uncompress.data.get(), uncompress.dataSize);
		fsave.close();

	}
	
	char key;
	std::cin >> key;

	return 0;

	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	FT_Library library; /* 库的句柄 */
	FT_Face face; /* face对象的句柄 */

	auto error = FT_Init_FreeType(&library);
	if (error) { std::cout << "初始化失败"; }


	const wchar_t* fontname = L"msyh";

	error = FT_New_Face(library,
		"C:/Windows/Fonts/msyh.ttc", //'ARIALN.TTF'
		0,
		&face);

	if (error == FT_Err_Unknown_File_Format)
	{
		std::cout << " 可以打开和读这个文件，但不支持它的字体格式" << std::endl;
	}
	else if (error)
	{
		std::cout << " 其它的错误码意味着这个字体文件不能打开和读，或者简单的说它损坏了..." << std::endl;
	}

	/*一个face对象包含该face的全部全局描述信息。通常的，这些数据可以通过分别查询句柄来直接访问，例如face->num_glyphs。

	FT_FaceRec结构描述包含了可用字段的完整列表。我们在这里详细描述其中的某些：

	Num_glyphs
	这个值给出了该字体face中可用的字形(glyphs)数目。简单来说，一个字形就是一个字符图像。但它不一定符合一个字符代码。

	Flags
	一个32位整数，包含一些用来描述face特性的位标记。例如，标记FT_FACE_FLAG_SCALABLE用来指示该face的字体格式是可伸缩并且该字形图像可以渲染到任何字符象素尺寸。要了解face标记的更多信息，请阅读“FreeType 2 API 参考”。

	Units_per_EM
	这个字段只对可伸缩格式有效，在其他格式它将会置为0。它指示了EM所覆盖的字体单位的个数。

	Num_fixed_size
	这个字段给出了当前face中嵌入的位图的个数。简单来说，一个strike就是某一特定字符象素尺寸下的一系列字形图像。例如，一个字体face可以包含象素尺寸为10、12和14的strike。要注意的是即使是可伸缩的字体格式野可以包含嵌入的位图！

	Fixed_sizes
	一个指向FT_Bitmap_Size成员组成的数组的指针。每一个FT_Bitmap_Size指示face中的每一个strike的水平和垂直字符象素尺寸。
	注意，通常来说，这不是位图strike的单元尺寸。*/



	//对于特定face中与字符大小相关的信息，FreeType 2使用size对象来构造。例如，当字符大小为12点时，使用一个size对象以1 / 64象素为单位保存某些规格（如ascender或者文字高度）的值。

	//	当FT_New_Face或它的亲戚被调用，它会自动在face中新建一个size对象，并返回。该size对象可以通过face->size直接访问。

	//	注意：一个face对象可以同时处理一个或多个size对象，但只有很少程序员需要用到这个功能，因而，我们决定简化该API，（例如，每个face对象只拥有一个size对象）但是这个特性我们仍然通过附加的函数提供。

	//	当一个新的face对象建立时，对于可伸缩字体格式，size对象默认值为字符大小水平和垂直均为10象素。对于定长字体格式，这个大小是未定义的，这就是你必须在装载一个字形前设置该值的原因。

	//	使用FT_Set_Char_Size完成该功能。这里有一个例子，它在一个300x300dpi设备上把字符大小设置为16pt。

	std::cout << "units_per_EM " << face->units_per_EM << std::endl;

	//error = FT_Set_Char_Size(
	//	face, /* face对象的句柄 */
	//	0, /* 以1/64点为单位的字符宽度 */
	//	16 * 64, /* 以1/64点为单位的字符高度 */
	//	300, /* 设备水平分辨率 */
	//	300); /* 设备垂直分辨率 */

	/*注意：

		* 字符宽度和高度以1 / 64点为单位表示。一个点是一个1 / 72英寸的物理距离。通常，这不等于一个象素。

		* 设备的水平和垂直分辨率以每英寸点数(dpi)为单位表示。显示设备（如显示器）的常规值为72dpi或96dpi。这个分辨率是用来从字符点数计算字符象素大小的。

		* 字符宽度为0意味着“与字符高度相同”，字符高度为0意味着“与字符宽度相同”。对于其他情况则意味着指定不一样的字符宽度和高度。

		* 水平或垂直分辨率为0时表示使用默认值72dpi。

		* 第一个参数是face对象的句柄，不是size对象的。

		这个函数计算对应字符宽度、高度和设备分辨率的字符象素大小。然而，如果你想自己指定象素大小，你可以简单地调用FT_Set_Pixel_Sizes，就像这样：
*/

	error = FT_Set_Pixel_Sizes(
		face, /* face对象句柄 */
		0, /* 象素宽度 */
		128); /* 象素高度 */

	//注意这两个函数都返回错误码。通常，错误会发生在尝试对定长字体格式（如FNT或PCF）设置不在face->fixed_size数组中的象素尺寸值。

	/*通常，一个应用程序想通过字符码来装载它的字形图像。字符码是一个特定编码中代表该字符的数值。例如，字符码64代表了ASCII编码中的’A’。

		一个face对象包含一个或多个字符表(charmap)，字符表是用来转换字符码到字形索引的。例如，很多TrueType字体包含两个字符表，一个用来转换Unicode字符码到字形索引，
		另一个用来转换Apple Roman编码到字形索引。这样的字体既可以用在Windows（使用Unicode）和Macintosh（使用Apple Roman）。同时要注意，一个特定的字符表可能没有覆盖完字体里面的全部字形。

		当新建一个face对象时，它默认选择Unicode字符表。如果字体没包含Unicode字符表，
		
		FreeType会尝试在字形名的基础上模拟一个。注意，如果字形名是不标准的那么模拟的字符表有可能遗漏某些字形。对于某些字体，包括符号字体和旧的亚洲手写字体，Unicode模拟是不可能的。

		我们将在稍后叙述如何寻找face中特定的字符表。现在我们假设face包含至少一个Unicode字符表，
		并且在调用FT_New_Face时已经被选中。我们使用FT_Get_Char_Index把一个Unicode字符码转换为字形索引，如下所示：

		glyph_index = FT_Get_Char_Index(face, charcode);

		这个函数会在face里被选中的字符表中查找与给出的字符码对应的字形索引。如果没有字符表被选中，这个函数简单的返回字符码。

		注意，这个函数是FreeType中罕有的不返回错误码的函数中的一个。然而，当一个特定的字符码在face中没有字形图像，函数返回0。
		按照约定，它对应一个特殊的字形图像――缺失字形，通常会显示一个框或一个空格。


		一旦你获得了字形索引，你便可以装载对应的字形图像。在不同的字体中字形图像存储为不同的格式。
		对于固定尺寸字体格式，如FNT或者PCF，每一个图像都是一个位图。对于可伸缩字体格式，如TrueType或者Type1，
		使用名为轮廓(outlines)的矢量形状来描述每一个字形。
		一些字体格式可能有更特殊的途径来表示字形（如MetaFont――但这个格式不被支持）。
		幸运的，FreeType2有足够的灵活性，可以通过一个简单的API支持任何类型的字形格式。

		字形图像存储在一个特别的对象――字形槽(glyph slot)中。
		就如其名所暗示的，一个字形槽只是一个简单的容器，
		它一次只能容纳一个字形图像，可以是位图，可以是轮廓，或者其他。
		每一个face对象都有一个字形槽对象，可以通过face->glyph来访问。
		它的字段在FT_GlyphSlotRec结构的文档中解释了。

		通过调用FT_Load_Glyph来装载一个字形图像到字形槽中，如下：

*/
	
	

	/*load_flags的值是位标志集合，是用来指示某些特殊操作的。其默认值是FT_LOAD_DEFAULT即0。

		这个函数会设法从face中装载对应的字形图像：

		* 如果找到一个对应该字形和象素尺寸的位图，那么它将会被装载到字形槽中。嵌入的位图总是比原生的图像格式优先装载。
		
		因为我们假定对一个字形，它有更高质量的版本。这可以用FT_LOAD_NO_BITMAP标志来改变。

		* 否则，将装载一个该字形的原生图像，把它伸缩到当前的象素尺寸，并且对应如TrueType和Type1这些格式，也会完成hinted操作。

		字段face->glyph->format描述了字形槽中存储的字形图像的格式。
		
		如果它的值不是FT_GLYPH_FORMAT_BITMAP，
		
		你可以通过FT_Render_Glyph把它直接转换为一个位图。如下：

*/
	

	/*render_mode参数是一个位标志集合，用来指示如何渲染字形图像。
	
	把它设为FT_RENDER_MODE_NORMAL渲染出一个高质量的抗锯齿(256级灰度)位图。
		
	这是默认情况，如果你想生成黑白位图，可以使用FT_RENDER_MODE_MONO标志。
*/


	/*一旦你生成了一个字形图像的位图，你可以通过glyph->bitmap(一个简单的位图描述符)直接访问，
	
	同时用glyph->bitmap_left和glyph->bitmap_top来指定起始位置。

		要注意，bitmap_left是从字形位图当前笔位置到最左边界的水平距离，
		
		而bitmap_top是从笔位置（位于基线）到最高边界得垂直距离。他么是正数，指示一个向上的距离。

		下一部分将给出字形槽内容的更多细节，以及如何访问特定的字形信息（包括度量）。*/
/*

	我们以slot->advance增加笔位置，slot->advance符合字形的步进宽度
	（也就是通常所说的走格(escapement)）。步进矢量以象素的1 / 64为单位表示，并且在每一次迭代中删减为整数象素。
*/
	//MakeImage(face, 'A');


	FT_ULong  charcode;
	FT_UInt   gindex;

	auto use_kerning = FT_HAS_KERNING(face);
	FT_ULong previous_charcode = 0;
	FT_UInt  previous_glyph = 0;

	

	charcode = FT_Get_First_Char(face, &gindex);
	while (gindex != 0)
	{
		//枚举所有的charcode
			charcode = FT_Get_Next_Char(face, charcode, &gindex);



			MakeImage(face, charcode,fontname);


			auto glyph_index = FT_Get_Char_Index(face, charcode);  //0x0E39

			
			if (use_kerning && previous_glyph > 0)
			{
				FT_Vector delta;

				FT_Get_Kerning(face, previous_glyph, glyph_index,
					FT_KERNING_DEFAULT, &delta);

				std::cout << "kerning: " << 
					std::hex << std::setw(4) 
					<< std::setfill('0') << previous_charcode << " " << charcode
					<< std::dec<< " x:" << delta.x <<" y:" << delta.y << std::endl   ;

			}
			previous_glyph = glyph_index;
			previous_charcode = charcode;
	}



	char a;
	std::cin >> a;
	
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return 0;
}





int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}


void MakeImage(FT_Face face, FT_ULong charcode,const wchar_t* fontname )
{
	auto glyph_index = FT_Get_Char_Index(face, charcode);  //0x0E39

	auto error = FT_Load_Glyph(
		face, /* face对象的句柄 */
		glyph_index, /* 字形索引 */
		FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP); /* 装载标志，参考下面 */
	
	error = FT_Render_Glyph(face->glyph, /* 字形槽 */
		FT_RENDER_MODE_NORMAL); /* 渲染模式 */

	FT_GlyphSlot slot;
	slot = face->glyph;

	std::cout << "advance.x " << (slot->advance.x / 64) << std::endl;
	std::cout << "advance.y " << (slot->advance.y / 64) << std::endl;

	if (charcode == 0x0e31)
	{
		std::cout << slot->advance.x;
	}


	
	
	
	auto bitmap = slot->bitmap;
	auto buffer = bitmap.buffer;

	int imageWidth = 128;// slot->advance.x / 64;//要生成的图片的大小
	int imageHeight = 128;  //face->size->metrics.height / 64;



	int detx = (imageWidth - bitmap.width) / 2;// -slot->bitmap_left;
	int dety = (imageHeight - bitmap.rows) / 2;// -slot->bitmap_top;

	int fontcenterx = detx - slot->bitmap_left;
	int fontcentery = dety + slot->bitmap_top;

	
	{
		Bitmap image(imageWidth, imageHeight);
		for (size_t j = 0; j < imageHeight; j++)
		{
			for (size_t i = 0; i < imageWidth; i++)
			{
				
				auto c = i - detx;
				auto r = j - dety;

				if (c >= 0 && r >= 0 && c < bitmap.width && r < bitmap.rows)
				{
					auto cc = buffer[r * bitmap.width + c];
					image.SetPixel(i, j, Color(cc, cc, cc));
				}
				else
				{
					image.SetPixel(i, j, Color(0, 0, 0));
				}

				if ( abs( (int)( i - fontcenterx))<3 && abs( (int)(j - fontcentery))<3 )
				{
					image.SetPixel(i, j, Color(255, 0, 0));
				}

			}
		}


		CLSID  encoderClsid;
		INT    result;
		WCHAR  strGuid[39];

		result = GetEncoderClsid(L"image/png", &encoderClsid);

		if (result < 0)
		{
			printf("The PNG encoder is not installed.\n");
		}
		else
		{
			std::wostringstream wstr;
			wstr << L"E:/CFL/Tools/FreeTypeTest/outputpng/" << fontname << "/" << std::hex << std::setw(4) << std::setfill(L'0') << charcode << ".png";

			image.Save(wstr.str().c_str(), &encoderClsid);
		}

	}


}
