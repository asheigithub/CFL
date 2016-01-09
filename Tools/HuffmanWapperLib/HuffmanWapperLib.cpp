// 这是主 DLL 文件。

#include "stdafx.h"

#include "HuffmanWapperLib.h"

namespace HuffmanWapperLib {

	HuffmanFileWapper^ Huffman::huffmanCompress(array<unsigned char>^ bytes)
	{
		/*auto p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(ret->bitmap, 0);
		memcpy(p.ToPointer(), fg.bitmap.get(), ret->imagewidth*ret->imageheight);*/

		//char* data = new char[bytes->Length];
		auto p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(bytes, 0);
		//memcpy(data, p.ToPointer(), bytes->Length);

		auto r = cfl::content::huffmanCompress((char*)p.ToPointer(), bytes->Length);

		//delete[] data;

		HuffmanFileWapper^ result = gcnew HuffmanFileWapper();

		HuffmanHeaderWapper^ header = gcnew HuffmanHeaderWapper();
		header->compressedlen = r.header.compressedlen;
		header->endiness = r.header.endiness;
		header->uncompresslen = r.header.uncompresslen;
		header->symbolsCount = r.header.symbolsCount;
		header->encodeTableRows = r.header.encodeTableRows;
		
		result->header = header;

		result->data = gcnew array< unsigned char >(r.getTotalFileLength());
		p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(result->data , 0);
		memcpy(p.ToPointer(), r.data.get()  , result->data->Length );

		return result;
	}

	array<unsigned char>^ Huffman::unHuffmanCompress(array<unsigned char>^ bytes)
	{
		//char* data = new char[bytes->Length];
		auto p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(bytes, 0);
		//memcpy(data, p.ToPointer(), bytes->Length);

		auto r = cfl::content::unHuffmanCompress((char*)p.ToPointer(), bytes->Length);
		//delete[] data;

		array<unsigned char>^ result = gcnew array<unsigned char>( r.dataSize );

		p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(result, 0);
		memcpy(p.ToPointer(), r.data.get(), r.dataSize);

		return result;

	}

	array<unsigned char>^ Huffman::unHuffmanCompress(
		array<unsigned char>^ bytes,
		DecodeInfoWapper^ decodeinfo
		)
	{
		auto p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(bytes, 0);

		auto symboltable = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(decodeinfo->symboltable, 0);

		cfl::content::EncodeSymbolTable* estable[256];
		for (size_t i = 0; i < decodeinfo->encodeTableRows; i++)
		{
			auto e = decodeinfo->encoderows[i];
			
			estable[i] = new cfl::content::EncodeSymbolTable();
			estable[i]->bitSize = e->bitSize;
			estable[i]->canonialhuffmancode = e->canonialhuffmancode;
			estable[i]->symbolCount = e->symbolCount;
			estable[i]->symbolIndex = e->symbolIndex;

		}


		auto r = cfl::content::doUnCompress(decodeinfo->uncompressedlen, 
			decodeinfo->encodeTableRows,
			estable, (unsigned char*)symboltable.ToPointer(), (unsigned char*)p.ToPointer());

		for (size_t i = 0; i < decodeinfo->encodeTableRows; i++)
		{
			delete estable[i];
		}

		array<unsigned char>^ result = gcnew array<unsigned char>(decodeinfo->uncompressedlen);

		p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(result, 0);
		memcpy(p.ToPointer(), r.get(), decodeinfo->uncompressedlen);

		return result;
	}



	System::Collections::Generic::List<PriorityItemWapper^>^
		Huffman::getPriority(array<unsigned char>^ bytes)
	{
		auto p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(bytes, 0);

		auto r = cfl::content::getPriority((char*)p.ToPointer(), bytes->Length);

		System::Collections::Generic::List<PriorityItemWapper^>^ result = gcnew System::Collections::Generic::List<PriorityItemWapper^>();

		for (size_t i = 0; i < r.size(); i++)
		{
			auto p = r[i];
			PriorityItemWapper^ item = gcnew PriorityItemWapper();
			item->character = p.datavalue;
			item->weight = p.weight;

			result->Add(item);

		}

		return result;
	}

	HuffmanFileWapper^ Huffman::huffmanCompress(array<unsigned char>^ bytes,
		System::Collections::Generic::List<PriorityItemWapper^>^ priority,
		bool skipheader)
	{
		
		auto p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(bytes, 0);
		

		std::vector<cfl::content::PriorityItem> priorlist;
		for (int i = 0; i < priority->Count ; i++)
		{
			cfl::content::PriorityItem p = cfl::content::PriorityItem();
			p.datavalue = priority[i]->character;
			p.weight = priority[i]->weight;

			priorlist.push_back(p);
		}


		auto r = cfl::content::huffmanCompress((char*)p.ToPointer(), bytes->Length,priorlist,skipheader);

		HuffmanFileWapper^ result = gcnew HuffmanFileWapper();

		HuffmanHeaderWapper^ header = gcnew HuffmanHeaderWapper();
		header->compressedlen = r.header.compressedlen;
		header->endiness = r.header.endiness;
		header->uncompresslen = r.header.uncompresslen;
		header->symbolsCount = r.header.symbolsCount;
		header->encodeTableRows = r.header.encodeTableRows;

		result->header = header;

		if (!skipheader)
		{
			result->data = gcnew array< unsigned char >(r.getTotalFileLength());
		}
		else
		{
			result->data = gcnew array< unsigned char >(r.header.compressedlen);
		}

		p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(result->data, 0);
		memcpy(p.ToPointer(), r.data.get(), result->data->Length);

		return result;


	}

	DecodeInfoWapper^ Huffman::loadDecodeInfo(
		array<unsigned char>^ bytes)
	{
		size_t oof = 0;
		size_t ocl = 0;

		auto p = System::Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(bytes, 0);
		auto info = cfl::content::loadDecodeInfo( (char*)p.ToPointer(),bytes->Length,oof,ocl );

		

		DecodeInfoWapper^ result = gcnew DecodeInfoWapper();
		result->encodeTableRows = info->encodeTableRows;
		
		result->outoffset = oof;
		result->uncompressedlen = ocl;

		for (size_t i = 0; i < 256; i++)
		{
			result->symboltable[i] = info->symboltable[i];
		}

		for (size_t i = 0; i < result->encodeTableRows ; i++)
		{
			auto r=gcnew EncodeSymbolTableWapper();

			r->bitSize = info->encoderows[i]->bitSize;
			r->canonialhuffmancode = info->encoderows[i]->canonialhuffmancode;
			r->symbolCount = info->encoderows[i]->symbolCount;
			r->symbolIndex = info->encoderows[i]->symbolIndex;

			result->encoderows[i] = r;
		}

		return result;

	}
}