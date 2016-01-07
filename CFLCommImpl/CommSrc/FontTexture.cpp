#include "headers/FontTexture.h"
#include "Content/CFLMemoryDataSource.h"
#include <mutex>

#include <vector>
#include <unordered_map>
#include <algorithm>

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

					std::shared_ptr<content::MemoryDataSource> texturedata,
					std::shared_ptr<Texture2DRef> textureRef,
					int col,
					int row)
					:
					fontdir(fontdir),
					fontpath(fontpath),
					imagedataoffset(imagedataoffset),
					texturedata(texturedata),
					textureRef(textureRef),
					col(col),
					row(row)
				{
					
				}


				cfl::file::DirType fontdir; 
				CFLString fontpath; 
				size_t imagedataoffset;

				std::shared_ptr<content::MemoryDataSource> texturedata;
				std::shared_ptr<Texture2DRef> textureRef;

				int col;
				int row;


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

			AddGlyphResut addGlyph(cfl::file::DirType fontdir, CFLString fontpath, size_t imagedataoffset)
			{
				int cols = FONT_TEXTURESIZE / 32;
				int rows = FONT_TEXTURESIZE / 32;

				int glyphidx = index % (cols * rows);

				std::unique_lock<std::mutex> lock(mtx);

				if (lastTexRef == nullptr || glyphidx == 0)
				{
					char* data = new char[FONT_TEXTURESIZE * FONT_TEXTURESIZE];

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


				toInitList.push_back(new initGlyph(fontdir,fontpath,imagedataoffset,lastdatasource,lastTexRef,col,row));

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

				std::sort(toInitList.begin(), toInitList.end(), 
					[](initGlyph* a,initGlyph* b )->bool{
					
					if (a->fontdir != b->fontdir)
					{
						return a->fontdir < b->fontdir;
					}
					else
					{
						return a->fontpath.getHashCode() < b->fontpath.getHashCode();
					}
				});


				cfl::file::FileData fd;
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

						fd.close();

						auto directony = cfl::file::getDirectory(toInitList[i]->fontdir);
						auto file = directony->getfile(toInitList[i]->fontpath.c_str());
						file->readFile(&fd);
					}

					content::BinaryReader br =
						content::BinaryReader(fd.data, 0, fd.filesize, cfl::content::little_endian);//必然是小端


					char buff[32*32];
					
					br.setPosition(toInitList[i]->imagedataoffset);
					br.readBytes( buff ,32 * 32);

					char* dst = static_cast<char*>(toInitList[i]->texturedata->getData());

					for (size_t j = 0; j < 32; j++)
					{
						memcpy(dst + 
							(toInitList[i]->row * 32 + j)*FONT_TEXTURESIZE + 
							toInitList[i]->col * 32, buff + j * 32, 32);
					}

					delete toInitList[i];

				}

				fd.close();

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
							0, false);
					}
					else
					{
						i->first->getTexture2D()->updateImageData(
							0, FONT_TEXTURESIZE, FONT_TEXTURESIZE,
							render::tex2d_inputformat::glluminace, 
							render::tex2d_pixeltype::glubyte, 
							i->second, 
							0, FONT_TEXTURESIZE);
					}
				}

				

			}
		}
	}
}
