#include "Graphic/CFLFont.h"
#include "Content/CFLBinaryReader.h"

#include <unordered_map>
#include <vector>

#include "CFLEntry.h"
#include "Graphic/CFLTexture.h"
#include "Content/CFLMemoryDataSource.h"

#include "headers/FontTexture.h"
#include "Content/CFLHuffman.h"

namespace cfl
{
	namespace graphic
	{
		namespace font
		{
			
			static std::unordered_map<CFLString, Font,string_hasher> fontMap;
			void Font::regFont(const Font font, const CFLString name)
			{
				fontMap.emplace(std::make_pair(name, font));
			}

			Font Font::getFont(const CFLString name)
			{
				auto f = fontMap.find(name);
				if (f != fontMap.end())
				{
					return f->second;
				}
				else
				{
					throw new std::runtime_error("Font Not Found");
				}
			}

			void Font::clearFonts()
			{
				clearFontTexture();
				fontMap.clear();
				

			}

			void readyFontGlyphImage(fontInnderData* data,Glyph* glyph);


			struct kerningkey
			{
				kerningkey(unsigned int c1,unsigned int c2)
					:code1(c1), code2(c2)
				{
					
				}

				unsigned int code1 = 0;
				unsigned int code2 = 0;

				bool operator==(const kerningkey& rhs) const
				{
					return code1 == rhs.code1 && code2 == rhs.code2;
				}

			};
			struct kerningkey_hasher
			{
				size_t operator()(const kerningkey key) const
				{
					std::hash<unsigned int> hasher;
					return hasher(key.code1) ^ (hasher(key.code2) << 1);
				}
			};

			

			struct fontInnderData
			{
				fontInnderData(FontInfo& info)
					:info(info),
					keringtable(nullptr),
					glyphtable(nullptr)
				{
					
				}
				~fontInnderData()
				{
					if (keringtable)
					{ 
						for (auto b = keringtable->begin(); b != keringtable->end(); b++)
						{
							delete b->second;
						}

						delete keringtable;
						keringtable = nullptr;
					}

					if (glyphtable)
					{
						for (auto b = glyphtable->begin(); b != glyphtable->end(); b++)
						{
							delete b->second;
						}

						delete glyphtable;
						glyphtable = nullptr;
					}

				}

				cfl::file::DirType dir; 
				CFLString path;

				FontInfo info;
				std::unordered_map<kerningkey, Kerning*,kerningkey_hasher>* keringtable;

				std::unordered_map<unsigned int, Glyph*>* glyphtable;

			};


			
			
			


			Font::Font(FontInfo& info):
				innerData(new fontInnderData(info))
			{
				
			}

			FontInfo* Font::getInfo() const
			{
				return &innerData->info;
			}

			Kerning* Font::queryKerning(unsigned int charcode1, unsigned int charcode2) const
			{
				if (innerData->info.useKerning)
				{
					if (innerData->keringtable)
					{
						auto f = innerData->keringtable->find(kerningkey(charcode1, charcode2));
						if (f != innerData->keringtable->end())
						{
							return f->second;
						}
						else
						{
							return nullptr;
						}
					}
					else
					{
						return nullptr;
					}
				}
				else
				{
					return nullptr;
				}
			}


			Glyph* Font::getGlyphForRending(unsigned int charcode) const
			{
				auto f = innerData->glyphtable->find(charcode);
				if (f != innerData->glyphtable->end())
				{
					if (f->second->glyphImage.isNull())
					{
						readyFontGlyphImage(innerData.get(), f->second);
					}
					return f->second;
				}
				else
				{
					return nullptr;
				}
			}


			Font Font::init(cfl::file::DirType dir, CFLString path)
			{
				auto directony = cfl::file::getDirectory(dir);
				auto file = directony->getfile(path.c_str());
				
				cfl::file::FileData fd;
				file->readFile(&fd);

				//读取字体文件信息
				content::BinaryReader br=
					content::BinaryReader( fd.data,0,fd.filesize, cfl::content::little_endian );//必然是小端

				FontInfo info;
				info.ascender = br.readShort();
				info.descender = br.readShort();
				info.bbox_xMax = br.readInt();
				info.bbox_xMin = br.readInt();
				info.bbox_yMax = br.readInt();
				info.bbox_yMin = br.readInt();
				info.height = br.readInt();
				info.units_per_EM = br.readInt();
				info.makeImageSize = br.readInt();

				info.q = br.readDouble();
				info.d = br.readDouble();


				info.glyphCount = br.readInt();
					
				int keringcount = br.readInt();

				//***字距微调表***
				std::unordered_map<kerningkey, Kerning*, kerningkey_hasher>*
					keringmap = nullptr;

				if (keringcount > 0)
				{
					info.useKerning = true;
					
					keringmap = new std::unordered_map<kerningkey, Kerning*, kerningkey_hasher>();
					
					for (int i = 0; i < keringcount; i++)
					{

						Kerning* kerndata = new Kerning();
						auto charcode1 = br.readUnsignedInt();
						auto charcode2 = br.readUnsignedInt();
						kerndata->detx = br.readInt() >> 6;
						kerndata->dety = br.readInt() >> 6;

						kerningkey key(charcode1,charcode2);
						
						keringmap->emplace(std::make_pair(key, kerndata));
					}

				}

				auto ret = Font(info);
				ret.innerData->keringtable = keringmap;
				ret.innerData->dir = dir;
				ret.innerData->path = path;
				

				std::vector<Glyph*> glyphlist;

				//***字形信息表***
				for (size_t i = 0; i < info.glyphCount; i++)
				{
					Glyph* g=new Glyph();
					g->charcode = br.readUnsignedInt(); //bw.Write(sdf.code)
					g->advance_x = br.readInt() >> 6; //bw.Write(sdf.glyph.advance_x)
					g->advance_y = br.readInt() >> 6; //bw.Write(sdf.glyph.advance_y)
					g->padleft = br.readInt(); //bw.Write(sdf.glyph.padleft)
					g->padtop = br.readInt(); //bw.Write(sdf.glyph.padtop)
					g->pen_x = br.readInt(); //bw.Write(sdf.glyph.pen_x)
					g->pen_y = br.readInt(); //bw.Write(sdf.glyph.pen_y)
					g->clipwidth = br.readInt(); //bw.Write(sdf.glyph.clipwidth)
					g->clipheight = br.readInt(); //bw.Write(sdf.glyph.clipheight)
					g->imagewidth = br.readInt(); //bw.Write(sdf.glyph.imagewidth)
					g->imageheight = br.readInt(); //bw.Write(sdf.glyph.imageheight)

					g->bytesize = br.readShort();
					
					glyphlist.push_back(g);
				}

				ret.innerData->glyphtable = new std::unordered_map<unsigned int, Glyph*>();

				auto offset = br.getPosition();
				for (size_t i = 0; i < info.glyphCount; i++)
				{
					glyphlist[i]->offset = offset;
					offset += glyphlist[i]->bytesize; //32*32;

					ret.innerData->glyphtable->emplace(std::make_pair(glyphlist[i]->charcode, glyphlist[i]));
				}

				glyphlist[0]->isfirstglyph = true;

				//读取第一个字符的哈夫曼编码表.后面所有字符用的都是同一个哈夫曼编码表
				auto huffmanLen = br.readShort();
				auto zeroandgrouplen = br.readShort();

				size_t outoffset = 0;
				size_t uncompressedlen = 0;
				ret.getInfo()->decodeinfo = cfl::content::huffman::loadDecodeInfo(fd.data + glyphlist[0]->offset + 4,
					huffmanLen, outoffset, uncompressedlen
					);
				
				
				fd.close();

				return ret;

			}


			Glyph* Font::getGlyph(unsigned int charcode) const
			{
				auto f = innerData->glyphtable->find(charcode);
				if (f != innerData->glyphtable->end())
				{
					return f->second;
				}
				else
				{
					return nullptr;
				}
			}





			

			//准备字体纹理
			void readyFontGlyphImage(fontInnderData* data, Glyph* glyph)
			{
				

				//auto directony = cfl::file::getDirectory( data->dir );
				//auto file = directony->getfile(data->path.c_str());

				//cfl::file::FileData fd;
				//file->readFile(&fd);

				////读取字体文件信息
				//content::BinaryReader br =
				//	content::BinaryReader(fd.data, 0, fd.filesize, cfl::content::little_endian);//必然是小端


				//char* buff = new char[32 * 32];
				//
				//br.setPosition(glyph->offset);
				//br.readBytes( buff ,32 * 32);

				//fd.close();
				
				auto addresult = addGlyph(data->dir,data->path,glyph->offset, glyph->bytesize,
					glyph->isfirstglyph,data->info.decodeinfo, data->info.q,data->info.d);
				auto tref = addresult.texRef;

				std::shared_ptr<GameImageData> gimgData = std::make_shared<GameImageData>();
				

				auto detx = glyph->padleft / 2;
				auto dety = glyph->padtop / 2;

				glyph->padleft -= detx;
				glyph->padtop -= dety;

				glyph->clipwidth += detx * 2;  //data->info.makeImageSize;
				glyph->clipheight += dety * 2; //data->info.makeImageSize;
				if (glyph->padleft + glyph->clipwidth > data->info.makeImageSize)
				{
					glyph->clipwidth = data->info.makeImageSize - glyph->padleft;
				}
				if (glyph->padtop + glyph->clipheight > data->info.makeImageSize)
				{
					glyph->clipheight = data->info.makeImageSize - glyph->padtop;
				}


				gimgData->refTexture = std::shared_ptr<Texture2DRef>(tref);
				gimgData->clipHeight = glyph->clipheight ;
				gimgData->clipWidth = glyph->clipwidth ;
				gimgData->height = glyph->imagewidth;
				gimgData->width = glyph->imageheight;
				gimgData->leftTopU = (addresult.intex_x * 1.0f / FONT_TEXTURESIZE) + 32.0f / FONT_TEXTURESIZE * glyph->padleft *1.0f / glyph->imagewidth ;
				gimgData->leftTopV = (addresult.intex_y * 1.0f / FONT_TEXTURESIZE) + 32.0f / FONT_TEXTURESIZE * glyph->padtop *1.0f / glyph->imageheight;
				gimgData->padLeft = glyph->padleft;
				gimgData->padTop = glyph->padtop;
				gimgData->rightBottomU = (addresult.intex_x * 1.0f / FONT_TEXTURESIZE) + 32.0f / FONT_TEXTURESIZE *(glyph->padleft + glyph->clipwidth) * 1.0f / glyph->imagewidth;
				gimgData->rightBottomV = (addresult.intex_y * 1.0f / FONT_TEXTURESIZE) + 32.0f / FONT_TEXTURESIZE *(glyph->padtop + glyph->clipheight) * 1.0f / glyph->imageheight;
				
				glyph->glyphImage = GameImage(gimgData);


			}







		}
	}
}
