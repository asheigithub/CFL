#include "Content/CFLContent.h"

#include "Content/CFLBinaryCache.h"
#include "CFLEntry.h"

#include "Content/CFLBinaryReader.h"
#include <vector>

#include <unordered_map>

namespace cfl
{
	namespace content
	{
		using namespace graphic;


		static std::unordered_map<const CFLString, std::shared_ptr<Texture2DRef>, string_hasher> texturefiles;

		static std::unordered_map<const CFLString, std::shared_ptr<GameImageData>, string_hasher > gameImagesDict;

		void Content::clearData()
		{
			texturefiles.clear();
			gameImagesDict.clear();
		}

		bool Content::isExists(const CFLString& imgpath)
		{
			auto f= gameImagesDict.find(imgpath);
			if (f != gameImagesDict.end())
			{
				return true;
			}
			else
			{
				return false;
			}

		}

		graphic::GameImage Content::loadGameImage(const CFLString& imgpath)
		{
			auto f = gameImagesDict.find(imgpath);
			if (f != gameImagesDict.end())
			{
				return GameImage(f->second) ;
			}
			else
			{
				return GameImage(nullptr);
			}
		}



		void Content::initGameImageFromFile(const file::DirType dirtype,
			const CFLString& cfgfile, const CFLString& basedir)
		{
			auto data= BinaryCache::getInstance()->getData(dirtype, cfgfile);
			if (data.getStatus() != file::FileIoStatus::success)
			{
				LOGE("合图文件读取失败!%s\n", data.getResoucePath().log_str());
				return;
			}

			BinaryReader br = BinaryReader(data.getData(), 0, data.getReadSize(), Endian::little_endian);

			if (br.readInt() == 0)
			{
				auto version = br.readInt();
				if (version == 3)
				{
					//先读取纹理集//
					std::vector<CFLString> texpath;
					auto texcount = br.readInt();

					for (int i = 0; i < texcount; i++)
					{
						auto l  = br.readInt();
						texpath.push_back(br.readUTFBytes(l));
					}

					auto linecount = br.readInt();

					std::vector<GameImageData> imageInDirList;
					auto dt = cfl::getTimer();

					
					initbyBinaryReader(br, dirtype ,basedir, linecount, imageInDirList, texpath);

					
					
					LOGI("usetimer: %d\n",cfl::getTimer() - dt);

				}
				else
				{
					LOGE("合图文件格式已不支持%s\n", data.getResoucePath().log_str());
					return;
				}
			}
			else
			{
				LOGE("合图文件格式已不支持 %s\n", data.getResoucePath().log_str());
				return;
			}

		}

		void  Content::initbyBinaryReader(BinaryReader br, const file::DirType dirtype,
			const CFLString& basedir,
			size_t linecount, 
			std::vector<GameImageData>& imageInDirList,
			const std::vector<CFLString>& texpathlist)
		{
			for (size_t i = 0; i < linecount; i++)
			{
				auto lc = br.readInt();
				if (lc > 11)
				{


					GameImageData img;

					auto sl  = br.readInt();
					
					
					CFLString f0 = br.readUTFBytes(sl);
					CFLString key = basedir  + f0;

					
					img.width = br.readShort();
					img.height = br.readShort();
					img.clipWidth = br.readShort();
					img.clipHeight = br.readShort();
					img.padLeft = br.readShort();
					img.padTop = br.readShort();

					auto textureX  = br.readDouble();
					auto textureY  = br.readDouble();

					auto f9 = br.readInt();
					
					auto rtf = texpathlist[f9];

					auto texwidth = br.readShort();
					auto texheight = br.readShort();

					auto imgw  = br.readShort();
					auto imgh  = br.readShort();

					img.leftTopU = (float)textureX / (texwidth);
					img.leftTopV = (float)textureY / (texheight);

					/*if (fontslist != null)
					{
						img.RightBottomV = (textureY + img.ClipHeight) / (texheight);
						img.RightBottomU = (textureX + img.ClipWidth) / (texwidth);
					}
					else*/
					{
						img.rightBottomV = (float)(textureY + imgh) / (texheight);
						img.rightBottomU = (float)(textureX + imgw) / (texwidth);
					}

					auto texturefilepath = basedir + rtf;


					std::shared_ptr<Texture2DRef> texRef;

					auto f = texturefiles.find(texturefilepath);
					if (f == texturefiles.end())
					{
						texRef = std::shared_ptr<Texture2DRef>(new  Texture2DRef(dirtype, texturefilepath));	
						texturefiles.emplace(std::make_pair(texturefilepath, texRef));
					}
					else
					{
						texRef = f->second;
					}
					
					img.refTexture = texRef;

					//将图片加入缓存
					auto imgiter = gameImagesDict.find(key);
					if (imgiter == gameImagesDict.end())
					{
						gameImagesDict.emplace(std::make_pair(key, std::make_shared<GameImageData>(img)));
					}
					else
					{
						LOGE("重复的GameImage %s\n", key.log_str());
					}
				}
				
			}

		}
		

	}
}