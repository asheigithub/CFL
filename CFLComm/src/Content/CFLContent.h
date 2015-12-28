#ifndef CFL_CONTENT_H
#define CFL_CONTENT_H

#include "../Graphic/CFLGameImage.h"
#include "../CFLFile.h"

namespace cfl
{
	namespace content
	{

		/*
		主要用于从合图中提取图片资源等
		*/
		class Content
		{
		public:
			//从一个文件中初始化合图信息
			static void initGameImageFromFile(const file::DirType dirtype, 
				const CFLString& cfgfile, const CFLString& basedir);

			//根据路径获取GameImage
			static graphic::GameImage loadGameImage(const CFLString& imgpath);

			//测试图片是否存在
			static bool isExists(const CFLString& imgpath);

			//清理数据
			static void clearData();

		private:
			Content()=default;
			~Content()=default;

			static void  initbyBinaryReader(content::BinaryReader br,
				const file::DirType dirtype,
				const CFLString& basedir,
				size_t linecount,
				std::vector<graphic::GameImageData>& imageInDirList,
				const std::vector<CFLString>& texpathlist);

		};

		

	}
}


#endif // !CFL_CONTENT_H
