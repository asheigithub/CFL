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
		�����ļ�·�����Ȼ������ֵ
		*/
		#define CFL_MAXFILEPATH 1024
		#define CFL_FILEBUFFSIZE 1024*512


		




		/*
		�洢Ŀ¼ö��
		*/
		enum DirType
		{
			/*
			assetĿ¼ ��һ��ֻ��������д���ݣ�
			*/
			asset,
			/*
			app�ڲ��洢Ŀ¼
			*/
			appStorage,
			/*
			app�ⲿ�洢Ŀ¼(sd����)
			*/
			document
		};

		/*
		�ļ���Ϣ
		*/
		class FileInfo;

		/*
		��ȡ���ļ�����
		*/
		struct FileData;

#ifdef ANDROID
		struct innerType;
#endif
		/*
		�첽�ļ���ȡ�ķ��ض���
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
		�ļ���Ϣ���������ָ��
		*/
		using FileInfoPtr =std::shared_ptr<FileInfo> ;

		/*
		��ǰϵͳ�Ĵ洢Ŀ¼��Ϣ
		*/
		class DirectoryInfo
		{
			friend FileInfo;

			friend void initDirecionaryInfo(void* ctx);
			friend DirectoryInfo* getDirectory(DirType type);

		public:		
			/*
			�ӵ�ǰĿ¼��ȡһ���ļ�����
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
		�ļ���Ϣ
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
			�ļ��Ƿ����
			*/
			bool isExists();

			/*
			ɾ���ļ�
			*/
			bool deletefile();


			/*
			�ļ���С�������ʧ���򷵻�-1
			*/
			int getFileLength();

			/*
			�ļ�ȫ�������ڴ�
			*/
			bool readFile(FileData* buff);

			using readFileProgress = void(*)(FileData*); //�ļ���ȡ�����¼�����

			/*
			�첽��ȡһ���ļ�
			*/
			ReadFileAsync readFileAsync(FileData* buff, readFileProgress progress);
			//asyncFunc::AsyncHandler<FileData*> readFileAsync(FileData* buff, readFileProgress progress);


			/*
			д���ļ�
			*/
			size_t writeFile(const char* buff, size_t count);

			/*
			���ļ���
			����ļ���ʧ���򷵻�nullptr
			*/
			std::shared_ptr<fileStream> openFileStreamForRead();

		private:
			FileInfo(const DirectoryInfo* dir, const char* file);
			std::string filepath;

			const int basedirlen;
			const DirectoryInfo* const basedir;

		};

		enum FileIoStatus  //�ļ���д���
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