#ifndef CFL_HUFFMAN_H
#define CFL_HUFFMAN_H

#include <memory>
#include <vector>
namespace cfl
{
	namespace content
	{


		//huffman编码压缩头
		struct HuffmanHeader
		{
			unsigned int endiness = 0x01020304;//大小端标志
			unsigned int uncompresslen=0;//解压后长度
			unsigned short symbolsCount=0;//符号数量
			unsigned short encodeTableRows = 0;//符号表行数
			unsigned int compressedlen=0;//压缩数据长度


		};
		//huffman压缩文件
		struct HuffmanFile
		{
			HuffmanFile()
				:header(),
				data(nullptr)
			{}

			HuffmanHeader header;
			std::shared_ptr<char> data;

			size_t getTotalFileLength() const
			{
				
				return sizeof(HuffmanHeader) + 
					sizeof(unsigned char) * 2 * header.encodeTableRows
					+ sizeof(unsigned char) * header.symbolsCount
					+ header.compressedlen;
				
				
			}

		};
		//解压后的数据
		struct UnHuffmanData
		{
			UnHuffmanData()
				:data(nullptr)
				, dataSize(0)
			{
				
			}

			std::shared_ptr<char> data=nullptr;
			size_t dataSize=0;

		};

		//词频项目
		struct PriorityItem
		{
			//字符
			unsigned char datavalue;
			//出现数量
			unsigned int  weight;
		};


		//编码符号表
		struct EncodeSymbolTable
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

		//解码需要的符号信息
		struct DecodeInfo
		{
			DecodeInfo() :encodeTableRows(0)
			{
				memset(symboltable, 0, 256);
			}

			~DecodeInfo()
			{
				if (encodeTableRows > 0)
				{
					for (size_t i = 0; i < encodeTableRows; i++)
					{
						delete encoderows[i];
					}
				}
			}

			unsigned short encodeTableRows;
			
			EncodeSymbolTable* encoderows[256];

			unsigned char symboltable[256];
			

			

		};



		//从要压缩的数据中创建词频表
		std::vector<PriorityItem> getPriority(const char* bytes, size_t len);


		std::shared_ptr<DecodeInfo> loadDecodeInfo(const char* bytes, size_t len, size_t& outoffset,size_t& uncompressedlen );

		HuffmanFile huffmanCompress(const char* bytes, size_t len);

		HuffmanFile huffmanCompress(const char* bytes, size_t len, std::vector<PriorityItem> priority ,bool skipheader=false );



		UnHuffmanData unHuffmanCompress(const char* bytes, size_t len);

		//使用指定的编码符号表和符号表解码数据
		std::shared_ptr<char> doUnCompress(
			unsigned int uncompresslen,
			unsigned short encodeTableRows,
			EncodeSymbolTable** const encoderows,
			unsigned char* symboltable,
			const unsigned char* data);
		
	}
}



#endif // !CFL_HUFFMAN_H
