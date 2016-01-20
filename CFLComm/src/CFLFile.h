#ifndef _CFLFile_H
#define _CFLFile_H

#ifndef CFL_API
#ifdef WIN32
#define CFL_API  __cdecl
#define CFL_CALLBACK  __cdecl
#else
#define CFL_API
#define CFL_CALLBACK
#endif
#endif

#include <string>
#include <memory>
//#include "CFLAsyncFunc.h"

#include "CFLFileStream.h"

namespace cfl
{
	namespace file
	{
		/*
		定义文件路径长度缓存最大值
		*/
		#define CFL_MAXFILEPATH 1024
		#define CFL_FILEBUFFSIZE 1024*512


		




		/*
		存储目录枚举
		*/
		enum DirType
		{
			/*
			asset目录 （一般只读不往里写数据）
			*/
			asset,
			/*
			app内部存储目录
			*/
			appStorage,
			/*
			app外部存储目录(sd卡等)
			*/
			document
		};

		/*
		文件信息
		*/
		class FileInfo;

		/*
		读取的文件数据
		*/
		struct FileData;

#ifdef ANDROID
		struct innerType;
#endif
		/*
		异步文件读取的返回对象
		*/
		struct ReadFileAsync
		{
#ifndef ANDROID
			struct innerType;
#endif
			ReadFileAsync(innerType inner);


			 bool hasCamplate();
			 void setCallbackFun(void(*callbacker)(ReadFileAsync&));
			 FileData* endInvoke() const;

		private:
			std::shared_ptr<innerType> inner;
		};

		/*
		文件信息对象的智能指针
		*/
		using FileInfoPtr =std::shared_ptr<FileInfo> ;

		/*
		当前系统的存储目录信息
		*/
		class DirectoryInfo
		{
			friend FileInfo;

			friend void initDirecionaryInfo(void* ctx);
			friend DirectoryInfo* getDirectory(DirType type);

		public:		
			/*
			从当前目录获取一个文件对象
			*/
			FileInfoPtr getfile(const char* filepath) const;


			static const char* CFL_API getAssetPath(){ return assetPath.c_str(); };
			static const char* CFL_API getAppStoragePath(){ return appStoragePath.c_str(); };
			static const char* CFL_API getDocumentPath(){ return documentPath.c_str(); };

		private:
			DirectoryInfo(DirType DirType);
			//DirectoryInfo(const DirectoryInfo& rhs) = delete;
			//DirectoryInfo& operator=(const DirectoryInfo& rhs) = delete;
			

			
			DirType _DirType;
			const char* dirPath;


			static std::string  assetPath;
			static std::string  appStoragePath;
			static std::string  documentPath;

		};
		
		void CFL_API initDirecionaryInfo(void* ctx);
		DirectoryInfo* CFL_API getDirectory(DirType type);

		/*
		文件信息
		*/
		class FileInfo
		{
			friend DirectoryInfo;
			
		public:
			//~FileInfo();
			

			const char* CFL_API getFilePath()
			{
				return filepath.c_str();
			}

			/*
			文件是否存在
			*/
			bool isExists();

			/*
			删除文件
			*/
			bool deletefile();


			/*
			文件大小，如果打开失败则返回-1
			*/
			int getFileLength();

			/*
			文件全部读进内存
			*/
			bool readFile(FileData* buff);

			using readFileProgress = void(*)(FileData*); //文件读取进度事件函数

			/*
			异步读取一个文件
			*/
			ReadFileAsync readFileAsync(FileData* buff, readFileProgress progress);
			//asyncFunc::AsyncHandler<FileData*> readFileAsync(FileData* buff, readFileProgress progress);


			/*
			写入文件
			*/
			size_t writeFile(const char* buff, size_t count);

			/*
			打开文件流
			如果文件打开失败则返回nullptr
			*/
			std::shared_ptr<fileStream> openFileStreamForRead();

		private:
			FileInfo(const DirectoryInfo* dir, const char* file);
			std::string filepath;

			const int basedirlen;
			const DirectoryInfo* const basedir;

		};

		enum FileIoStatus  //文件读写结果
		{
			notstart,
			reading,
			success,       
			fileNotFound,
			fileReadError
		};

		struct FileData
		{
			size_t filesize = 0;
			FileIoStatus status = FileIoStatus::notstart;
			size_t length=0;
			char* data=nullptr;
			void close()
			{
				if (data)
				{
					delete[] data;
					data = nullptr;
				}
			}
		};
	}

}




#endif