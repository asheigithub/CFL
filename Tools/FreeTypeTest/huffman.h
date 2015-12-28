#ifndef CFL_HUFFMAN_H
#define CFL_HUFFMAN_H

#include <memory>

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



		HuffmanFile huffmanCompress(const char* bytes, size_t len);

		UnHuffmanData unHuffmanCompress(const char* bytes, size_t len);

		
	}
}



#endif // !CFL_HUFFMAN_H
