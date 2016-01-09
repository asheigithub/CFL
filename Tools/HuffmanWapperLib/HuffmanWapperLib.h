// HuffmanWapperLib.h

#pragma once

using namespace System;

#include "huffman.h"
namespace HuffmanWapperLib {

	public ref struct HuffmanHeaderWapper
	{
		unsigned int endiness = 0x01020304;//大小端标志
		unsigned int uncompresslen = 0;//解压后长度
		unsigned short symbolsCount = 0;//符号数量
		unsigned short encodeTableRows = 0;//符号表行数
		unsigned int compressedlen = 0;//压缩数据长度
	};

	//huffman压缩文件
	public ref struct HuffmanFileWapper
	{
		HuffmanFileWapper()
			:header(nullptr),
			data(nullptr)
		{}
		
		HuffmanHeaderWapper^ header;
		
		array<unsigned char>^ data;

		size_t getTotalFileLength()
		{
			return sizeof(cfl::content::HuffmanHeader) +
				sizeof(unsigned char) * 2 * header->encodeTableRows
				+ sizeof(unsigned char) * header->symbolsCount
				+ header->compressedlen;
		}

	};

	

	//词频项目
	public ref struct PriorityItemWapper
	{
		//字符
		unsigned char character;
		//出现数量
		unsigned int  weight;
	};


	//编码符号表
	public ref struct EncodeSymbolTableWapper
	{
		//位长
		unsigned char bitSize = 0;
		//该位长的符号数量
		unsigned char symbolCount = 0;
		//首编码符号索引
		size_t symbolIndex = 0;
		//首编码符号
		unsigned int canonialhuffmancode = 0;

	};


	public ref struct DecodeInfoWapper
	{

		unsigned short encodeTableRows;

		array<EncodeSymbolTableWapper^>^ encoderows = gcnew array<EncodeSymbolTableWapper^>(256);

		array<unsigned char>^ symboltable = gcnew array<unsigned char>(256) ;

		size_t outoffset;
		size_t uncompressedlen;
	};


	public ref class Huffman
	{
	public:

		HuffmanFileWapper^ huffmanCompress(array<unsigned char>^ bytes);

		array<unsigned char>^ unHuffmanCompress(array<unsigned char>^ bytes);



		//从要压缩的数据中创建词频表
		System::Collections::Generic::List<PriorityItemWapper^>^ 
			getPriority(array<unsigned char>^ bytes);

		HuffmanFileWapper^ huffmanCompress(array<unsigned char>^ bytes,
			System::Collections::Generic::List<PriorityItemWapper^>^ priority)
		{
			return huffmanCompress(bytes, priority, false);
		}

		HuffmanFileWapper^ huffmanCompress(array<unsigned char>^ bytes,
			System::Collections::Generic::List<PriorityItemWapper^>^ priority,
			bool skipheader);


		DecodeInfoWapper^ loadDecodeInfo(
			array<unsigned char>^ bytes);

		//使用指定的编码符号表和符号表解码数据
		array<unsigned char>^ unHuffmanCompress(
			array<unsigned char>^ bytes,
			DecodeInfoWapper^ decodeinfo
			);

	};

}
