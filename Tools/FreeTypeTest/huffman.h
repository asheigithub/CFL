#ifndef CFL_HUFFMAN_H
#define CFL_HUFFMAN_H

#include <memory>

namespace cfl
{
	namespace content
	{
		//huffman����ѹ��ͷ
		struct HuffmanHeader
		{
			unsigned int endiness = 0x01020304;//��С�˱�־
			unsigned int uncompresslen=0;//��ѹ�󳤶�
			unsigned short symbolsCount=0;//��������
			unsigned short encodeTableRows = 0;//���ű�����
			unsigned int compressedlen=0;//ѹ�����ݳ���


		};
		//huffmanѹ���ļ�
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
		//��ѹ�������
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
