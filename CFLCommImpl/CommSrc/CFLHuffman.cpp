#include <vector>
#include <list>
#include <string>

#include <algorithm>  
#include <functional>
#include <iostream>
#include <stack>
#include <iomanip>

#include <bitset>



#include "Content/CFLHuffman.h"


namespace cfl
{
	namespace content
	{
		namespace huffman
		{

			struct HuffmanNode
			{
				//频率
				unsigned	int weight = 0;
				//字节内容
				unsigned	char bytevalue = 0;

				//父节点
				HuffmanNode* parent = nullptr;

				//左子节点
				HuffmanNode* left = nullptr;
				//右子节点
				HuffmanNode* right = nullptr;


				unsigned int depth = 0;

				//如果是子节点则标识左或者右  0为左，1为右
				int          leftorright = 0;

				//范式哈夫曼编码
				unsigned int canonialhuffmancode = 0;

				//编码码字
				std::string strHuffmanCode;

			};



			bool selectNode(std::list<HuffmanNode*>* priority, HuffmanNode*& one, HuffmanNode*& two)
			{
				auto n1 = priority->begin();
				if (n1 != priority->end() && *n1)
				{
					one = *n1;

					priority->erase(n1);
				}
				else
				{
					return false;
				}
				auto n2 = priority->begin();
				if (n2 != priority->end() && *n2)
				{
					two = *n2;

					priority->erase(n2);
				}
				else
				{
					return false;
				}

				return true;

			}


			HuffmanNode* makeHuffmanTree(std::list<HuffmanNode*>* priority)
			{

				HuffmanNode* root = nullptr;

				HuffmanNode* n1 = nullptr;
				HuffmanNode* n2 = nullptr;
				while (selectNode(priority, n1, n2))
				{
					root = new HuffmanNode();


					n1->parent = root;
					n2->parent = root;

					n1->leftorright = 0;
					n2->leftorright = 1;

					root->left = n1;
					root->right = n2;

					root->weight = n1->weight + n2->weight;

					//将这个节点放回priority
					auto pos = priority->begin();
					for (; pos != priority->end() && *pos; pos++)
					{
						if ((*pos)->weight >= root->weight)
						{
							break;
						}
					}

					priority->insert(pos, root);

				}
				if (root)
				{
					return root;
				}
				else if (n1) //只有1个节点的情况
				{
					root = new HuffmanNode();

					root->left = n1;
					n1->parent = root;

					n1->leftorright = 0;

					return root;
				}
				else        //权重表是空的
				{
					return nullptr;
				}

			}

			void deleteNodes(HuffmanNode* root)
			{
				//清理内存
				if (root)
				{
					std::list<HuffmanNode*> todelete;

					std::stack<HuffmanNode*> s;
					HuffmanNode *p = root;
					while (p != nullptr || !s.empty())
					{
						while (p != NULL)
						{
							todelete.push_back(p);
							s.push(p);
							p = p->left;


						}
						if (!s.empty())
						{
							p = s.top();
							s.pop();
							p = p->right;
						}
					}

					for (auto i = todelete.begin(); i != todelete.end(); i++)
					{
						delete *i;
					}

				}


			}

			std::vector<PriorityItem> getPriority(const char* bytes, size_t len)
			{
				const unsigned char* input = reinterpret_cast<const unsigned char*>(bytes);

				std::vector<PriorityItem*> prioritylist;

				prioritylist.resize(256);
				size_t maxwidth = 0;
				//频率表
				for (size_t i = 0; i < len; i++)
				{
					auto v = input[i];
					auto nodeptr = prioritylist[v];
					if (!nodeptr)
					{
						nodeptr = new PriorityItem();
						prioritylist[v] = nodeptr;
					}

					nodeptr->datavalue = v;
					++nodeptr->weight;

					if (nodeptr->weight > maxwidth)
					{
						maxwidth = nodeptr->weight;
					}

				}

				//先按词频排序
				std::sort(prioritylist.begin(), prioritylist.end(),
					[](PriorityItem* node1, PriorityItem* node2)->bool{
					if (node1 == nullptr && node2 == nullptr)
					{
						return false;
					}

					if (node1 == nullptr)
					{
						return false;
					}
					else if (node2 == nullptr)
					{
						return true;
					}
					else
					{
						if (node1->weight == node2->weight)
						{
							return node1->datavalue < node2->datavalue;
						}
						else
						{
							return (node1->weight) < (node2->weight);
						}
					}
				});

				std::vector<PriorityItem> ret;
				for (size_t i = 0; i < 256 && prioritylist[i] != nullptr; i++)
				{
					ret.push_back(*prioritylist[i]);
					//std::cout << "'" << prioritylist[i]->datavalue << "' weight: " << prioritylist[i]->weight << std::endl;
					delete prioritylist[i];


				}




				return ret;
			}

			HuffmanFile huffmanCompress(const char* bytes, size_t len)
			{
				auto priority = getPriority(bytes, len);
				return huffmanCompress(bytes, len, priority, false);
			}

			HuffmanFile huffmanCompress(const char* bytes, size_t len, std::vector<PriorityItem> priority, bool skipheader)
			{
				const unsigned char* input = reinterpret_cast<const unsigned char*>(bytes);

				std::vector<HuffmanNode*> prioritylist;
				std::list<HuffmanNode*> temppriority;




				for (size_t i = 0; i < priority.size(); i++)
				{
					auto node = new HuffmanNode();
					node->bytevalue = priority[i].datavalue;
					node->weight = priority[i].weight;

					prioritylist.push_back(node);
					temppriority.push_back(node);
				}

				HuffmanNode* root = nullptr;
				root = makeHuffmanTree(&temppriority);
				//可能没有节点（说明为0字节）
				//更新码长
				for (auto pos = prioritylist.begin(); pos != prioritylist.end(); pos++)
				{
					auto node = *pos;
					auto p = node->parent;
					while (p)
					{
						node->depth++;
						p = p->parent;
					}
				}
				//按照以编码位长为第1关键字、符号为第2关键字进行排序
				std::sort(prioritylist.begin(), prioritylist.end(),
					[](HuffmanNode* node1, HuffmanNode* node2)->bool
				{
					if (node1->depth != node2->depth)
					{
						return (node1->depth < node2->depth);
					}
					else
					{
						return (node1->bytevalue < node2->bytevalue);
					}
				});

				/*
				生成范式哈夫曼码
				code = 0
				while more symbols :
				print symbol, code
				code = (code + 1) << ((bit length of the next symbol) - (current bit length))*/

				unsigned int canonialCode = 0;
				for (size_t i = 0; i < prioritylist.size(); i++)
				{
					auto node = prioritylist[i];
					std::bitset<32> bit(canonialCode);

					node->canonialhuffmancode = canonialCode;
					node->strHuffmanCode = bit.to_string().substr(32 - node->depth);

					/*std::cout << "symbol '" << node->bytevalue << "',bitsize:" << node->depth << " code: "
					<< node->strHuffmanCode << std::endl;*/

					if (i < prioritylist.size() - 1)
					{
						canonialCode = (canonialCode + 1) << ((prioritylist[i + 1])->depth - node->depth);
					}

				}

				//压缩//
				//生成压缩符号表头
				std::vector<unsigned char> symbolTable;
				for (size_t i = 0; i < prioritylist.size(); i++)
				{
					symbolTable.push_back(prioritylist[i]->bytevalue);
				}



				std::vector<EncodeSymbolTable> encodetable;
				size_t bitsize = 0;
				for (size_t i = 0; i < prioritylist.size(); i++)
				{
					auto node = prioritylist[i];
					if (bitsize < node->depth)
					{
						bitsize = node->depth;
						EncodeSymbolTable row;
						row.bitSize = node->depth;
						row.symbolIndex = i;
						encodetable.push_back(row);
					}

					auto tablerow = &encodetable.back();
					tablerow->symbolCount++;
				}
				//输出符号表
				for (size_t i = 0; i < encodetable.size(); i++)
				{
					auto row = encodetable[i];

					/*std::cout << "bitSize:" << (int)row.bitSize << " symbolCount:" << (int)row.symbolCount
					<< " symbolidx:" << row.symbolIndex << " firstSymbol: '" << symbolTable[row.symbolIndex] << "'"
					<< std::endl;*/

				}


				HuffmanHeader header;
				header.symbolsCount = static_cast<unsigned short>(prioritylist.size());
				header.encodeTableRows = static_cast<unsigned short>(encodetable.size());
				unsigned char tempbuff = 0;
				size_t fulled = 0;

				std::string* table[256];
				memset(table, 0, 256);
				for (size_t i = 0; i < prioritylist.size(); i++)
				{
					table[prioritylist[i]->bytevalue] = &(prioritylist[i]->strHuffmanCode);
				}

				unsigned char* output = new unsigned char[sizeof(header)
					+ sizeof(unsigned char) * 2 * header.encodeTableRows
					+ sizeof(unsigned char) * header.symbolsCount + len];

				size_t outlens = sizeof(header)
					+ sizeof(unsigned char) * 2 * header.encodeTableRows
					+ sizeof(unsigned char) * header.symbolsCount;


				if (!skipheader)
				{
					unsigned char* pos = output + sizeof(header);

					//写入符号
					for (size_t i = 0; i < header.symbolsCount; i++)
					{
						unsigned char w = symbolTable[i];
						memcpy(pos, &w, 1);
						pos++;
					}
					//写入编码表
					for (size_t i = 0; i < header.encodeTableRows; i++)
					{
						auto row = encodetable[i];

						memcpy(pos, &row.bitSize, 1);
						pos++;

						memcpy(pos, &row.symbolCount, 1);
						pos++;
					}
				}
				else
				{

					outlens = 0;
				}


				for (size_t i = 0; i < len; i++)
				{
					auto data = input[i];

					std::string* huffcodeptr = table[data];

					const char* code = huffcodeptr->c_str();
					for (size_t i = 0; i < huffcodeptr->length(); i++)
					{
						auto bit = code[i];
						if (bit == '1')
						{
							tempbuff = tempbuff | 1;
						}

						++fulled;
						if (fulled == 8)
						{
							//装入压缩后的数组
							output[outlens] = tempbuff;
							outlens++;

							fulled = 0;
							tempbuff = 0;
						}
						else
						{
							tempbuff = tempbuff << 1;
						}
					}
				}

				if (fulled > 0)//最后一个字节,左移动补足
				{
					tempbuff = tempbuff << (7 - fulled);
					output[outlens] = tempbuff;
					outlens++;

					fulled = 0;				//压缩完成
					tempbuff = 0;
				}

				if (!skipheader)
				{
					header.compressedlen = outlens - sizeof(header)
						- sizeof(unsigned char) * 2 * header.encodeTableRows
						- sizeof(unsigned char) * header.symbolsCount;
					header.uncompresslen = len;
					memcpy(output, &header, sizeof(header));

				}
				else
				{
					header.compressedlen = outlens;
					header.uncompresslen = len;
				}

				deleteNodes(root);


				HuffmanFile file;
				file.header = header;

				file.data = std::shared_ptr<char>(reinterpret_cast<char*>(output),
					[](char* d)
				{
					delete[] d;
				}
				);

				return file;
			}

			unsigned int readBits(const unsigned char* bytes, size_t toreadbits, size_t& stbit)
			{
				static unsigned char masktable[8] = {
					0xff,
					0xff >> 1,
					0xff >> 2,
					0xff >> 3,
					0xff >> 4,
					0xff >> 5,
					0xff >> 6,
					0xff >> 7
				};



				unsigned int buff = 0;

				size_t stbyte = stbit / 8;

				size_t stbyteBit = stbit % 8;

				size_t canreadBits = 8 - stbyteBit;

				buff = (*(bytes + stbyte)) &   masktable[stbyteBit];;//擦除高位stbyteBit个位



				int readless = static_cast<int>(toreadbits);

				while (readless > static_cast<int>(canreadBits))
				{
					buff = buff << 8;
					stbyte++;
					buff = buff | (*(bytes + stbyte));

					readless -= 8;
				}


				//再右移位调整
				stbit = stbit + toreadbits;

				auto rightshift = (8 - (stbit % 8)) % 8;
				buff = buff >> (rightshift);


				return buff;
			}





			std::shared_ptr<char> doUnCompress(
				unsigned int uncompresslen,
				unsigned short encodeTableRows,
				EncodeSymbolTable** const encoderows,
				unsigned char* symboltable,
				const unsigned char* data)
			{
				unsigned char* decompressed = new unsigned char[uncompresslen + 1];
				decompressed[uncompresslen] = '\0';

				if (uncompresslen > 0)
				{

					struct searchtable //快速搜索表
					{
						unsigned char datavalue;
						unsigned char bitSize;
					};

					int k = encoderows[encodeTableRows - 1]->bitSize;
					if (k > 12)
					{
						k = 12;
					}

					size_t ftable_size = 1 << k;
					searchtable* ftable = new searchtable[ftable_size];
					for (size_t i = 0; i < ftable_size; i++)
					{
						size_t j = 0;
						while (true)
						{
							auto row = encoderows[j];

							auto toreadbits = row->bitSize;

							unsigned int readdata = i;

							readdata = readdata >> (k - row->bitSize);

							auto Num = readdata - row->canonialhuffmancode;

							if (Num >= row->symbolCount)
							{
								j = j + 1;
								ftable[i].bitSize = row->bitSize;
								if (j == encodeTableRows || row->bitSize > k)
								{
									break;
								}
							}
							else
							{
								auto c = symboltable[row->symbolIndex + Num];

								ftable[i].datavalue = c;
								ftable[i].bitSize = row->bitSize;

								break;
							}
						}

					}




					/*建立完解码表就可以开始解码了。首先，设i = 0，从解码表的第i行开始。
					根据编码位长从数据中读取相应长度的位。将读取数据与首编码相减，假设差值等于Num。
					如果Num大于等于符号数量，那么将i加1重新开始整个过程。
					如果Num小于符号数量，那么将符号索引加Num从符号列表上的对应位置解码出一个符号。

					例如，输入数据“11110”。令i = 0，此时编码位长为2。
					读取2位的数据“11”与首编码相减等于3。3大于等于符号数量，于是i = i + 1等于1。
					此时编码位长为3。读取3位的数据“111”与首编码相减等于1。
					1大于等于符号数量，于是i = i + 1等于2。
					此时编码位长为5。读取5位的数据“11110”与首编码相减等于2。
					2小于符号数量，2加符号索引4等于6。
					从表2.3中可以查到序号为6的符号是“E”。从而解码出符号“E”。
					跳过当前已经解码的5位数据，可以重新开始解码下一个符号。*/

					size_t readstbit = 0;
					for (size_t upos = 0; upos < uncompresslen; upos++)
					{

						size_t i = 0;
						unsigned int _rb = 0;
						unsigned char _sub = 0;

						//先从表里查询
						auto fidx = readBits(data, k, readstbit);
						auto frest = ftable[fidx];

						if (frest.bitSize <= k)
						{
							readstbit -= k - frest.bitSize;
							decompressed[upos] = frest.datavalue;
							continue;
						}
						else
						{
							while (encoderows[i]->bitSize < frest.bitSize)
							{
								i++;
							}
							_rb = fidx;
							_sub = k;
						}

						while (true)
						{
							auto row = encoderows[i];
							auto toreadbits = row->bitSize - _sub;

							unsigned int readdata = (_rb << toreadbits) | readBits(data, toreadbits, readstbit);

							auto Num = readdata - row->canonialhuffmancode;

							if (Num >= row->symbolCount)
							{
								_rb = readdata;
								_sub = row->bitSize;

								i = i + 1;
								//readstbit -= toreadbits;


							}
							else
							{
								auto c = symboltable[row->symbolIndex + Num];
								decompressed[upos] = c;
								break;
							}
						}


					}


					delete[] ftable;



				}

				return std::shared_ptr<char>(reinterpret_cast<char*>(decompressed),
					[](char * d)
				{
					delete[] d;
				}
				);
			}

			std::shared_ptr<DecodeInfo> loadDecodeInfo(const char* bytes, size_t len, size_t& outoffset, size_t& uncompressedlen)
			{
				if (len < sizeof(HuffmanHeader))
				{
					return nullptr;
				}

				HuffmanHeader header;
				memcpy(&header, bytes, sizeof(HuffmanHeader));


				uncompressedlen = header.uncompresslen;

				std::shared_ptr<DecodeInfo> info = std::make_shared<DecodeInfo>();

				const unsigned char* pos = reinterpret_cast<const unsigned char*>(bytes)+sizeof(HuffmanHeader);
				//重建huffman树

				size_t offset = sizeof(HuffmanHeader);

				for (size_t i = 0; i < header.symbolsCount; i++)
				{
					unsigned char w;
					memcpy(&w, pos, 1);
					pos++;

					offset++;

					info->symboltable[i] = w;

				}
				int symbolidx = 0;

				for (size_t i = 0; i < header.encodeTableRows; i++)
				{
					EncodeSymbolTable* row = new EncodeSymbolTable();
					info->encoderows[i] = row;

					memcpy(&(row->bitSize), pos, 1);
					pos++; offset++;
					memcpy(&(row->symbolCount), pos, 1);
					pos++; offset++;

					row->symbolIndex = symbolidx;
					symbolidx += row->symbolCount;
				}

				info->encodeTableRows = header.encodeTableRows;


				//从符号表和编码表重建哈夫曼编码信息

				unsigned int dhuffmancode = 0;
				for (size_t i = 0; i < header.encodeTableRows; i++)
				{
					auto row = info->encoderows[i];
					row->canonialhuffmancode = dhuffmancode;
					dhuffmancode += row->symbolCount;

					if (static_cast<int>(i) < header.encodeTableRows - 1)
					{
						dhuffmancode = dhuffmancode << (info->encoderows[i + 1]->bitSize - row->bitSize);
					}
				}

				outoffset = offset;

				return info;
			}

			UnHuffmanData unHuffmanCompress(const char* bytes, size_t len)
			{
				size_t offset = 0; size_t uncompress = 0;
				auto dinfo = loadDecodeInfo(bytes, len, offset, uncompress);

				if (dinfo == nullptr)
				{
					return UnHuffmanData();
				}


				unsigned short encodeTableRows = dinfo->encodeTableRows;
				unsigned int   uncompresslen = uncompress;

				auto decompressdata = doUnCompress(uncompresslen,
					encodeTableRows, dinfo->encoderows, dinfo->symboltable, reinterpret_cast<const unsigned char*>(bytes)+offset);


				UnHuffmanData ret;
				ret.dataSize = uncompresslen;
				ret.data = decompressdata;

				return ret;
			}
		}

	}
}
