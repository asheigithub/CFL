#ifndef CFL_HUFFMAN_H
#define CFL_HUFFMAN_H

#include <memory>
#include <vector>
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

		//��Ƶ��Ŀ
		struct PriorityItem
		{
			//�ַ�
			unsigned char datavalue;
			//��������
			unsigned int  weight;
		};


		//������ű�
		struct EncodeSymbolTable
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

		//������Ҫ�ķ�����Ϣ
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



		//��Ҫѹ���������д�����Ƶ��
		std::vector<PriorityItem> getPriority(const char* bytes, size_t len);


		std::shared_ptr<DecodeInfo> loadDecodeInfo(const char* bytes, size_t len, size_t& outoffset,size_t& uncompressedlen );

		HuffmanFile huffmanCompress(const char* bytes, size_t len);

		HuffmanFile huffmanCompress(const char* bytes, size_t len, std::vector<PriorityItem> priority ,bool skipheader=false );



		UnHuffmanData unHuffmanCompress(const char* bytes, size_t len);

		//ʹ��ָ���ı�����ű�ͷ��ű��������
		std::shared_ptr<char> doUnCompress(
			unsigned int uncompresslen,
			unsigned short encodeTableRows,
			EncodeSymbolTable** const encoderows,
			unsigned char* symboltable,
			const unsigned char* data);
		
	}
}



#endif // !CFL_HUFFMAN_H
