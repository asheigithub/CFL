#include "stdafx.h"
#include "MyForm.h"
#include <memory>
#include "Content/CFLCTF.h"

using namespace cfl;

namespace WinUse2 
{
#pragma managed(push, off)

	void fullheader(int bitmapwidth, int bitmapheight, 
		content::CTFContentType contenttype,
		content::CTFAlphaType alphatype,  
		content::CTFHasAlpha hasalpha,
		unsigned int texdatalength,
		unsigned int separateLength,
		unsigned char* buff)
	{
		content::CTFHeader header;
		header.alphaType = alphatype;
		header.hasAlpha = hasalpha;
		header.originImageWidth = bitmapwidth;
		header.originImageHeight = bitmapheight;
		header.contentType = contenttype;
		header.width = bitmapwidth;
		header.height = bitmapheight;
		header.stride = bitmapwidth * 4;
		header.length = texdatalength ;
		header.separateLength = separateLength;


		memcpy(buff, &header, sizeof(header));

	}

#pragma managed(pop)	

	System::Void MyForm::btnCreateRGBATex_Click(System::Object^  sender, System::EventArgs^  e)
	{
		System::Drawing::Bitmap^ bitmap = gcnew System::Drawing::Bitmap(
			L"E:/CFL/Debug/testreadfile/commimage1/texture-0.png");

		System::IO::FileStream^ fs = gcnew System::IO::FileStream("E:/CFL/Debug/testreadfile/commimage1/texture-0.gif", 
			System::IO::FileMode::Create);
		System::IO::BinaryWriter^ bw = gcnew System::IO::BinaryWriter(fs);
		
		
		unsigned char buff[sizeof(content::CTFHeader)];
		fullheader(bitmap->Width,bitmap->Height,
			content::CTFContentType::rgba,
			content::CTFAlphaType::alphachannal,
			content::CTFHasAlpha::transparent,			
			bitmap->Width*bitmap->Height*4,0,
			buff);
		
		System::Collections::Generic::List<unsigned char>^ buffs = gcnew System::Collections::Generic::List<unsigned char>();
		
		for (size_t i = 0; i < sizeof(content::CTFHeader); i++)
		{
			buffs->Add(buff[i]);
		}


		bw->Write(buffs->ToArray(), 0, sizeof(content::CTFHeader));

		
		for (int i = 0; i < bitmap->Height; i++)
		{
			for (int j = 0; j < bitmap->Width; j++)
			{
				auto color = bitmap->GetPixel(j, i);

				bw->Write((unsigned char)color.R);
				bw->Write((unsigned char)color.G);
				bw->Write((unsigned char)color.B);
				bw->Write((unsigned char)color.A);

			}
		}
		
		bw->Close();
		fs->Close();

	}


	System::Void MyForm::btnCreateETCSeperate_Click(System::Object^  sender, System::EventArgs^  e)
	{
		System::IO::FileStream^ fs = 
			gcnew System::IO::FileStream(L"E:/CFL/Debug/testreadfile/commimage1/texture-0.ktx", System::IO::FileMode::Open);

		auto datalen = fs->Length;

		System::IO::FileStream^ alphafs =
			gcnew System::IO::FileStream(L"E:/CFL/Debug/testreadfile/commimage1/texture-0_alpha.ktx", System::IO::FileMode::Open);
		auto alphalen = alphafs->Length;



		System::IO::FileStream^ outfile = 
			gcnew System::IO::FileStream("E:/CFL/Debug/testreadfile/commimage1/texture-0.gif",
			System::IO::FileMode::Create);
		System::IO::BinaryWriter^ bw = gcnew System::IO::BinaryWriter(outfile);



		unsigned char buff[sizeof(content::CTFHeader)];
		fullheader(2048, 1024, 
			content::CTFContentType::ktx,
			content::CTFAlphaType::separate, 
			content::CTFHasAlpha::transparent,
			datalen , alphalen,
			buff);

		System::IO::BinaryReader^ br = gcnew System::IO::BinaryReader(fs);
		auto data = br->ReadBytes(datalen);
		br->Close();

		System::IO::BinaryReader^ alphabr = gcnew System::IO::BinaryReader(alphafs);
		auto alphadata = alphabr->ReadBytes(alphalen);
		alphabr->Close();


		System::Collections::Generic::List<unsigned char>^ buffs = gcnew System::Collections::Generic::List<unsigned char>();
		for (size_t i = 0; i < sizeof(content::CTFHeader); i++)
		{
			buffs->Add(buff[i]);
		}

		bw->Write(buffs->ToArray(), 0, sizeof(content::CTFHeader));

		bw->Write(data, 0, data->Length);
		bw->Write(alphadata, 0, data->Length);



		bw->Close();
		alphafs->Close();
		fs->Close();
	}

	System::Void MyForm::btnCreateAlatas_Click(System::Object^  sender, System::EventArgs^  e)
	{
		System::IO::FileStream^ fs =
			gcnew System::IO::FileStream(L"E:/CFL/Debug/testreadfile/commimage1/texture-2.ktx", System::IO::FileMode::Open);

		auto datalen = fs->Length;

		

		System::IO::FileStream^ outfile =
			gcnew System::IO::FileStream("E:/CFL/Debug/testreadfile/commimage1/texture-2.gif",
			System::IO::FileMode::Create);
		System::IO::BinaryWriter^ bw = gcnew System::IO::BinaryWriter(outfile);



		unsigned char buff[sizeof(content::CTFHeader)];
		fullheader(2048, 1024,
			content::CTFContentType::ktx,
			content::CTFAlphaType::atlas,
			content::CTFHasAlpha::transparent,
			datalen, 0,
			buff);

		System::IO::BinaryReader^ br = gcnew System::IO::BinaryReader(fs);
		auto data = br->ReadBytes(datalen);
		br->Close();

		

		System::Collections::Generic::List<unsigned char>^ buffs = gcnew System::Collections::Generic::List<unsigned char>();
		for (size_t i = 0; i < sizeof(content::CTFHeader); i++)
		{
			buffs->Add(buff[i]);
		}

		bw->Write(buffs->ToArray(), 0, sizeof(content::CTFHeader));

		bw->Write(data, 0, data->Length);
		


		bw->Close();
		
		fs->Close();
	}

	System::Void MyForm::btnKTXContent_Click(System::Object^  sender, System::EventArgs^  e)
	{
		System::IO::FileStream^ fs =
			gcnew System::IO::FileStream(L"E:/CFL/Debug/testreadfile/commimage1/texture-2.ktx", System::IO::FileMode::Open);

		auto datalen = fs->Length;



		System::IO::FileStream^ outfile =
			gcnew System::IO::FileStream("E:/CFL/Debug/testreadfile/commimage1/texture-2.gif",
			System::IO::FileMode::Create);
		System::IO::BinaryWriter^ bw = gcnew System::IO::BinaryWriter(outfile);



		unsigned char buff[sizeof(content::CTFHeader)];
		fullheader(2048, 1024,
			content::CTFContentType::ktx,
			content::CTFAlphaType::alphachannal,
			content::CTFHasAlpha::transparent,
			datalen, 0,
			buff);

		System::IO::BinaryReader^ br = gcnew System::IO::BinaryReader(fs);
		auto data = br->ReadBytes(datalen);
		br->Close();



		System::Collections::Generic::List<unsigned char>^ buffs = gcnew System::Collections::Generic::List<unsigned char>();
		for (size_t i = 0; i < sizeof(content::CTFHeader); i++)
		{
			buffs->Add(buff[i]);
		}

		bw->Write(buffs->ToArray(), 0, sizeof(content::CTFHeader));

		bw->Write(data, 0, data->Length);



		bw->Close();

		fs->Close();
	}

}