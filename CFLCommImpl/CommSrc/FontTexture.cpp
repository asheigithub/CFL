#include "headers/FontTexture.h"
#include "Content/CFLMemoryDataSource.h"
#include <mutex>

#include <vector>
#include <unordered_map>
#include <algorithm>

#include <math.h>
#include "headers/VLITable.h"

#include "headers/fidct.h"
#include "CFLFileStream.h"

#include "CFLEntry.h"

namespace cfl
{
	namespace graphic
	{
		namespace font
		{
			static std::shared_ptr<content::MemoryDataSource> lastdatasource=nullptr;
			static std::shared_ptr<Texture2DRef> lastTexRef = nullptr;

			static size_t index=0;

			static std::mutex mtx;

			


			struct initGlyph
			{
				initGlyph(
					cfl::file::DirType fontdir,
					CFLString fontpath,
					size_t imagedataoffset,
					size_t datasize,

					std::shared_ptr<content::MemoryDataSource> texturedata,
					std::shared_ptr<Texture2DRef> textureRef,
					int col,
					int row,
					bool isfirstglyph,
					std::shared_ptr<cfl::content::huffman::DecodeInfo> decodeinfo,

					double q,
					double d
					)
					:
					fontdir(fontdir),
					fontpath(fontpath),
					imagedataoffset(imagedataoffset),
					datasize(datasize),
					texturedata(texturedata),
					textureRef(textureRef),
					col(col),
					row(row),
					isfirstglyph(isfirstglyph),
					decodeinfo(decodeinfo),
					q(q),
					d(d)
				{
					
				}


				cfl::file::DirType fontdir; 
				CFLString fontpath; 
				size_t imagedataoffset;
				size_t datasize;

				std::shared_ptr<content::MemoryDataSource> texturedata;
				std::shared_ptr<Texture2DRef> textureRef;

				int col;
				int row;

				bool isfirstglyph;
				std::shared_ptr<cfl::content::huffman::DecodeInfo> decodeinfo;

				double q;
				double d;

			};

			static std::vector<initGlyph*> toInitList;


			void clearFontTexture()
			{
				for (size_t i = 0; i < toInitList.size(); i++)
				{
					delete toInitList[i];
				}
				toInitList.clear();
				lastTexRef = nullptr;
				lastdatasource = nullptr;
				index = 0;
			}

			AddGlyphResut addGlyph(cfl::file::DirType fontdir, CFLString fontpath, size_t imagedataoffset, size_t datasize,
				bool isfirstglyph,  std::shared_ptr<cfl::content::huffman::DecodeInfo> decodeinfo,
				double q,double d
				)
			{
				int cols = FONT_TEXTURESIZE / 32;
				int rows = FONT_TEXTURESIZE / 32;

				int glyphidx = index % (cols * rows);

				std::unique_lock<std::mutex> lock(mtx);

				if (lastTexRef == nullptr || glyphidx == 0)
				{
					char* data = new char[FONT_TEXTURESIZE * FONT_TEXTURESIZE];
					memset(data, 0, FONT_TEXTURESIZE*FONT_TEXTURESIZE);
					lastdatasource = std::make_shared<content::MemoryDataSource>(
						std::shared_ptr<char>(data, std::default_delete<char[]>()), FONT_TEXTURESIZE*FONT_TEXTURESIZE);

					render::textures::Texture2D* texture = new render::textures::Texture2D(
						render::tex2d_internalformat::glluminace, FONT_TEXTURESIZE, FONT_TEXTURESIZE
						);

					lastTexRef = std::make_shared<Texture2DRef>(
						std::shared_ptr<render::textures::Texture2D>(texture)
						);
				}

				int col = glyphidx % cols;
				int row = glyphidx / cols;

				index++;


				toInitList.push_back(new initGlyph(fontdir,fontpath,imagedataoffset,datasize,
					lastdatasource,lastTexRef,col,row,isfirstglyph,decodeinfo ,q,d ));

				//拷贝数据源
				/*char* dst = static_cast<char*>(lastdatasource->getData());

				for (size_t i = 0; i < 32; i++)
				{
					memcpy(dst + (row * 32 + i)*FONT_TEXTURESIZE + col * 32, glyphdata + i * 32, 32);
				}*/

				/*if (!lastTexRef->getTexture2D()->getIsDone())
				{
					lastTexRef->getTexture2D()->uploadFromImageData(0, FONT_TEXTURESIZE, FONT_TEXTURESIZE,
						render::tex2d_inputformat::glluminace, render::tex2d_pixeltype::glubyte, lastdatasource, FONT_TEXTURESIZE, 0, false);
				}
				else
				{
					lastTexRef->getTexture2D()->updateImageData(0, FONT_TEXTURESIZE, FONT_TEXTURESIZE,
						render::tex2d_inputformat::glluminace, render::tex2d_pixeltype::glubyte, lastdatasource, 0, FONT_TEXTURESIZE);
				}*/
				
				AddGlyphResut ret=AddGlyphResut( lastTexRef,col* 32, row *32 ) ;
				return ret;


			}

			void updateFontTexture()
			{
				if (toInitList.size() == 0)
				{
					return;
				}

				fidct_init();

				std::sort(toInitList.begin(), toInitList.end(), 
					[](initGlyph* a,initGlyph* b )->bool{
					
					if (a->fontdir != b->fontdir)
					{
						return a->fontdir < b->fontdir;
					}
					else
					{
						if (a->fontpath != b->fontpath)
						{
							return a->fontpath.getHashCode() < b->fontpath.getHashCode();
						}
						else
						{
							return a->imagedataoffset < b->imagedataoffset;
						}
						
					}
				});
				  
				std::shared_ptr<cfl::file::fileStream> fs;
				//cfl::file::FileData fd;
				cfl::file::DirType lastdir = cfl::file::DirType::asset;
				CFLString lastpath;

				std::unordered_map<Texture2DRef*, std::shared_ptr< content::MemoryDataSource>> toloadmap;

				for (size_t i = 0; i < toInitList.size(); i++)
				{
					toloadmap.emplace(toInitList[i]->textureRef.get(), toInitList[i]->texturedata);

					if (lastdir != toInitList[i]->fontdir ||
						lastpath != toInitList[i]->fontpath
						)
					{
						lastdir = toInitList[i]->fontdir;
						lastpath = toInitList[i]->fontpath;

						//fd.close();
						if (fs !=nullptr)
						{
							fs->dispose();
						}

						auto directony = cfl::file::getDirectory(toInitList[i]->fontdir);
						auto file = directony->getfile(toInitList[i]->fontpath.c_str());
						//file->readFile(&fd);
						fs = file->openFileStreamForRead();

					}

					content::BinaryReader br =
						content::BinaryReader(fs.get(), cfl::content::little_endian);//必然是小端

					
					br.setPosition(toInitList[i]->imagedataoffset);

					
					//解码字形数据
					auto huffmanLen = br.readShort();
					auto zeroandgrouplen = br.readShort();

					cfl::content::huffman::UnHuffmanData zeroandgroup;

					if (toInitList[i]->isfirstglyph)
					{
						char huffmanbuff[64 * 16];
						br.readBytes(huffmanbuff, huffmanLen);
						zeroandgroup = cfl::content::huffman::unHuffmanCompress(huffmanbuff, huffmanLen);
						//zeroandgroup=cfl::content::huffman::unHuffmanCompress(fd.data + br.getPosition(), huffmanLen);
					}
					else
					{
						char huffmanbuff[64 * 16];
						br.readBytes(huffmanbuff, huffmanLen);

						zeroandgroup.dataSize = zeroandgrouplen;
						zeroandgroup.data = cfl::content::huffman::doUnCompress(zeroandgrouplen,
							toInitList[i]->decodeinfo->encodeTableRows,
							toInitList[i]->decodeinfo->encoderows,
							toInitList[i]->decodeinfo->symboltable,
							//reinterpret_cast<unsigned char*>( fd.data) + br.getPosition());
							reinterpret_cast<unsigned char*>(huffmanbuff));

					}

					size_t vlisize = toInitList[i]->datasize - huffmanLen - 4;//sizeof(huffmanLen)+sizeof(zeroandgrouplen)
					//char* vlicode = fd.data + br.getPosition() + huffmanLen;
					char vlicode[64 * 16];
					br.readBytes(vlicode, vlisize);

					

					//br.readBytes( buff ,32 * 32);
					char buff[32 * 32];

					unDCT((unsigned char*)buff, zeroandgroup.data.get(), zeroandgroup.dataSize, vlicode, vlisize , toInitList[i]->q,toInitList[i]->d );



					char* dst = static_cast<char*>(toInitList[i]->texturedata->getData());

					for (size_t j = 0; j < 32; j++)
					{
						memcpy(dst + 
							(toInitList[i]->row * 32 + j)*FONT_TEXTURESIZE + 
							toInitList[i]->col * 32, buff + j * 32, 32);
					}

					delete toInitList[i];

				}

				//fd.close();
				if (fs != nullptr)
				{
					fs->dispose();
				}
				
				toInitList.clear();

				for (auto i = toloadmap.begin(); i != toloadmap.end(); i++)
				{
					if (!i->first->getTexture2D()->getIsDone())
					{
						i->first->getTexture2D()->uploadFromImageData(
							0, FONT_TEXTURESIZE, FONT_TEXTURESIZE,
							render::tex2d_inputformat::glluminace, 
							render::tex2d_pixeltype::glubyte, 
							i->second,
							FONT_TEXTURESIZE, 
							0, false,true);
					}
					else
					{
						i->first->getTexture2D()->updateImageData(
							0, FONT_TEXTURESIZE, FONT_TEXTURESIZE,
							render::tex2d_inputformat::glluminace, 
							render::tex2d_pixeltype::glubyte, 
							i->second, 
							0, FONT_TEXTURESIZE,true);
					}
				}

				

			}
		




			typedef short zigslist[16][64];

			zigslist* loadfromDctResult(char* zeroandgroup,
				unsigned int zeroandgrouplen, char* vlicode,
				unsigned int vlicodelen)
			{
				//std::vector<short*> ret;

				static zigslist ret;

				size_t p1idx = 0;
				size_t p2idx = 0;
				for (size_t toread = 0; toread < 16; toread++)
				{
					//short* result = new short[64];
					short* result = ret[toread];
					auto stbit = p1idx;

					unsigned char bits[64]; unsigned int zerocount[64]; unsigned char rlcbitcount = 0;

					size_t items = 0;
					while (true)
					{
						auto zeros = cfl::content::huffman::readBits((unsigned char*)zeroandgroup, 4, stbit);
						auto valuebits = cfl::content::huffman::readBits((unsigned char*)zeroandgroup, 4, stbit);
						items += zeros;
						if (valuebits > 0)
						{
							items +=1;
						}
						else if (zeros == 15)
						{
							items += 1;
						}

						zerocount[rlcbitcount] = zeros;
						bits[rlcbitcount] = valuebits;
						rlcbitcount += 1;

						if (items >= 64)
						{
							break;
						}
					}

					p1idx = stbit;

					stbit = p2idx;


					size_t resultidx = 0;
					for (size_t r = 0; r < rlcbitcount; r++)
					{
						for (size_t i = 0; i < zerocount[r]; i++)
						{
							result[resultidx] = 0; ++resultidx;
						}

						if (bits[r] > 0)
						{
							auto value = cfl::content::huffman::readBits( (unsigned char*)vlicode,bits[r],stbit );

							result[resultidx] = cfl::vli::getValue(bits[r],value);
							++resultidx;
						}
						else if (zerocount[r] == 15 && resultidx < 64)
						{
							result[resultidx] = 0;
							++resultidx;
						}
					}

					p2idx = stbit % 8 == 0 ? stbit : stbit + (8 - (stbit % 8)); 
					
					//	ret.Add(result.ToArray())
					//ret.push_back(result);
				}
				return &ret;
			}


			typedef short block[8][8];

			void unZigzag(short* zig,block* block)
			{
				short zigzagtable[8][8] = {
					{ 0, 1, 5, 6, 14, 15, 27, 28 },
					{ 2, 4, 7, 13, 16, 26, 29, 42 },
					{ 3, 8, 12, 17, 25, 30, 41, 43 },
					{ 9, 11, 18, 24, 31, 40, 44, 53 },
					{ 10, 19, 23, 32, 39, 45, 52, 54 },
					{ 20, 22, 33, 38, 46, 51, 55, 60 },
					{ 21, 34, 37, 47, 50, 56, 59, 61 },
					{ 35, 36, 48, 49, 57, 58, 62, 63 }
				};
				for (size_t i = 0; i < 8; i++)
				{
					for (size_t j = 0; j < 8; j++)
					{
						(*block)[i][j] = zig[zigzagtable[i][j]];
					}
				}
			}

			typedef float blockf[8][8];

			void idct(block* data, blockf* output)
			{
				const double PI = 3.1415926;
				const size_t NUM = 8;
				double ALPHA;
				double BETA;
				short u = 0, v = 0, i = 0, j = 0;

				for (size_t i = 0; i < NUM; i++)
				{
					for (size_t j = 0; j < NUM; j++)
					{
						double tmp = 0;
						for ( u = 0; u < NUM; u++)
						{
							for ( v = 0; v < NUM; v++)
							{
								if (u == 0)
								{
									ALPHA = sqrtf(1.0f / NUM);
								}
								else
								{
									ALPHA = sqrtf(2.0f / NUM);
								}
								if (v == 0)
								{
									BETA = sqrtf(1.0f / NUM);
								}
								else
								{
									BETA = sqrtf(2.0 / NUM);
								}

								tmp += ALPHA * BETA * (*data)[u][v] * cosf((2.0f * i + 1.0f) * u * PI / (2.0f * NUM))
									* cosf((2.0f * j + 1.0f) * v * PI / (2.0 * NUM));
							}
						}


						
						(*output)[i][j] = roundf(tmp);

					}
				}
			}


			void unDCT(unsigned char* result, char* zeroandgroup,
				unsigned int zeroandgrouplen, char* vlicode,
				unsigned int vlicodelen, double q, double d)
			{
				float qt[8][8];
				for (size_t i = 0; i < 8; i++)
				{
					for (size_t j = 0; j < 8; j++)
					{
						qt[i][j] = static_cast<float>( pow(1 + (i + 1 + j + 1 - 1), q) * d / 2);
					}
				}

				
				auto zigs = *loadfromDctResult(zeroandgroup, zeroandgrouplen, vlicode, vlicodelen);

				

				for (size_t k = 0; k < 16; k++)
				{
					block data;
					unZigzag(zigs[k], &data);
					for (size_t c = 0; c < 8; c++)
					{
						for (size_t r = 0; r < 8; r++)
						{
							auto v = data[c][r] * qt[c][r];

							data[c][r] =(short)( v);
						}
					}

					//blockf idctbyte;// = idct(data, 8)
					//idct(&data, &idctbyte);
					fidct(data[0]);


					auto i = (k % 4) * 8;
					auto j = (k / 4) * 8;

					for (size_t col = 0; col < 8; col++)
					{
						for (size_t row = 0; row < 8; row++)
						{
							auto value = data[col][row]+128;
							
							if (value < 0)
							{
								result[(j + row) * 32 + (i + col)] = 0;
							}
							else if (value >255)
							{
								result[(j + row) * 32 + (i + col)] = 255;
							}
							else
							{
								result[(j + row) * 32 + (i + col)] = (unsigned char)value;
							}
							
						}
					}

					//delete[] zigs[k];
				}


			}

		}
	}
}
