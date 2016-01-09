// HuffmanWapperLib.h

#pragma once

using namespace System;

#include "huffman.h"
namespace HuffmanWapperLib {

	public ref struct HuffmanHeaderWapper
	{
		unsigned int endiness = 0x01020304;//��С�˱�־
		unsigned int uncompresslen = 0;//��ѹ�󳤶�
		unsigned short symbolsCount = 0;//��������
		unsigned short encodeTableRows = 0;//���ű�����
		unsigned int compressedlen = 0;//ѹ�����ݳ���
	};

	//huffmanѹ���ļ�
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

	

	//��Ƶ��Ŀ
	public ref struct PriorityItemWapper
	{
		//�ַ�
		unsigned char character;
		//��������
		unsigned int  weight;
	};


	//������ű�
	public ref struct EncodeSymbolTableWapper
	{
		//λ��
		unsigned char bitSize = 0;
		//��λ���ķ�������
		unsigned char symbolCount = 0;
		//�ױ����������
		size_t symbolIndex = 0;
		//�ױ������
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



		//��Ҫѹ���������д�����Ƶ��
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

		//ʹ��ָ���ı�����ű�ͷ��ű��������
		array<unsigned char>^ unHuffmanCompress(
			array<unsigned char>^ bytes,
			DecodeInfoWapper^ decodeinfo
			);

	};

}
