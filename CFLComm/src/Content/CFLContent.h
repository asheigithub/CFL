#ifndef CFL_CONTENT_H
#define CFL_CONTENT_H

#include "../Graphic/CFLGameImage.h"
#include "../CFLFile.h"

namespace cfl
{
	namespace content
	{

		/*
		��Ҫ���ڴӺ�ͼ����ȡͼƬ��Դ��
		*/
		class Content
		{
		public:
			//��һ���ļ��г�ʼ����ͼ��Ϣ
			static void initGameImageFromFile(const file::DirType dirtype, 
				const CFLString& cfgfile, const CFLString& basedir);

			//����·����ȡGameImage
			static graphic::GameImage loadGameImage(const CFLString& imgpath);

			//����ͼƬ�Ƿ����
			static bool isExists(const CFLString& imgpath);

			//��������
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
