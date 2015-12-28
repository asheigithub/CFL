#include "clearwindows.h"
#include "CFLEntry.h"
#include "CFLFile.h"
#include <string>

#include <android_native_app_glue.h>
#include <jni.h>
#include "jnitool.h"

#include <stdarg.h>
#include <sys/stat.h>
#include <dirent.h>

#include "CFLAsyncFunc.h"


extern "C" jint
Java_com_app_CFLActivity_activityonStart(JNIEnv* env, jobject thiz)
{
	return 0;
}


namespace cfl
{
	namespace file
	{
		
		using namespace cfl::asyncFunc;

		

		//**定义返回类**
		struct innerType : AsyncHandler<FileData*>
		{
			innerType(AsyncHandler<FileData*> v) :
			AsyncHandler<FileData*>(v)
			{

			}
		};
		static std::allocator<ReadFileAsync> readfileasync_alloc;

		typedef void(*ReadFileCallBackFun)(ReadFileAsync&);

		static std::allocator<ReadFileCallBackFun> readfilecallbackfun_alloc;


		ReadFileAsync::ReadFileAsync(innerType in) :inner(std::make_shared<innerType>(std::move(in)))
		{
		}

		bool ReadFileAsync::hasCamplate(){ return inner->hasCamplate(); }
		void ReadFileAsync::setCallbackFun(
			void(*callbacker)(ReadFileAsync&))
		{

			auto v = [](AsyncHandler<FileData*>& arg)
			{
				void* tagptr = *(arg.tag);

				auto tagarg = reinterpret_cast<std::tuple<ReadFileAsync*, ReadFileCallBackFun*>*>(tagptr);
				auto obj = std::get<0>(*tagarg);
				auto func = std::get<1>(*tagarg);

				(*func)(*obj);


				readfileasync_alloc.destroy(obj);
				readfileasync_alloc.deallocate(obj, 1);

				readfilecallbackfun_alloc.destroy(func);
				readfilecallbackfun_alloc.deallocate(func, 1);

				delete tagarg;

				//delete arg.tag;
			};


			auto tuple = new std::tuple<ReadFileAsync*, ReadFileCallBackFun*>(
				readfileasync_alloc.allocate(1), readfilecallbackfun_alloc.allocate(1));

			*(inner->tag) = tuple;
			readfileasync_alloc.construct(std::get<0>(*tuple), *this);
			readfilecallbackfun_alloc.construct(std::get<1>(*tuple), callbacker);

			inner->setCallbackFun(v);
		}
		FileData* ReadFileAsync::endInvoke() const
		{
			return inner->endInvoke();
		}

		//****

		std::string  DirectoryInfo::assetPath;
		std::string  DirectoryInfo::appStoragePath;
		std::string  DirectoryInfo::documentPath;

		static android_app* application=nullptr;

		//初始化系统目录
		void initDirecionaryInfo(void* ctx)
		{
			android_app* app = reinterpret_cast<android_app*>(ctx);
			application = app;

			static bool inited = false;
			if (!inited)
			{
				inited = true;

				DirectoryInfo::assetPath = "assets";
				
				android_app* state = reinterpret_cast<android_app*>(ctx);

				// 获取ANativeActivity，这个类其实也是调用JAVA的类，这里通过NDK入口的传入参数获取。作用是窗口Activity的

				ANativeActivity* pActivity = state->activity;


				auto jnia = attachCurrentThread(state);
				if (!jnia.success)
				{
					DirectoryInfo::appStoragePath ="null";
					DirectoryInfo::documentPath = "null";

					LOGE("jni attach thread failed\n");
					return;
				}

				// 通过Activity里面getPackageCodePath的方法获取程序运行路径，通过JNI用C++搞JAVA
				JNIEnv* env = jnia.env;
				jclass clazz = env->GetObjectClass(pActivity->clazz);
				{
					jmethodID methodID = env->GetMethodID(clazz, "getFilesDir", "()Ljava/io/File;");

					jobject jfile = env->CallObjectMethod(pActivity->clazz, methodID);

					jclass jfileclass = env->GetObjectClass(jfile);
					jmethodID jfilepath = env->GetMethodID(jfileclass, "getPath", "()Ljava/lang/String;");
					jobject result = env->CallObjectMethod(jfile, jfilepath);

					jboolean isCopy;
					auto m_pStrBuf = env->GetStringUTFChars((jstring)result, &isCopy);
					auto m_nStrLen = env->GetStringLength((jstring)result);
					//__android_log_print(LOGINFO_INFO, "android_main", "Code Path =%d %s", strPath.m_nStrLen, strPath.m_pStrBuf);
					
					
					DirectoryInfo::appStoragePath = m_pStrBuf;
					env->ReleaseStringUTFChars((jstring)result, m_pStrBuf);

				}
				{


					jclass environment = env->FindClass("android/os/Environment");

					jmethodID getESD = env->GetStaticMethodID(environment, "getExternalStorageDirectory", "()Ljava/io/File;");

					jobject jfile = env->CallStaticObjectMethod(environment, getESD);

					jclass jfileclass = env->GetObjectClass(jfile);
					jmethodID jfilepath = env->GetMethodID(jfileclass, "getPath", "()Ljava/lang/String;");
					jobject result = env->CallObjectMethod(jfile, jfilepath);

					jboolean isCopy;
					auto m_pStrBuf = env->GetStringUTFChars((jstring)result, &isCopy);
					auto m_nStrLen = env->GetStringLength((jstring)result);
					
					
					DirectoryInfo::documentPath = m_pStrBuf;
					

					env->ReleaseStringUTFChars((jstring)result, m_pStrBuf);
				}

				detachCurrentThread(jnia);


			}


		}


		DirectoryInfo* getDirectory(DirType type)
		{
			
			static DirectoryInfo dir1(DirType::appStorage);
			static DirectoryInfo dir2(DirType::asset);
			static DirectoryInfo dir3(DirType::document);

			switch (type)
			{
			case cfl::file::asset:
				return  &dir2;
				break;
			case cfl::file::appStorage:
				return  &dir1;
				break;
			case cfl::file::document:
				return  &dir3;
				break;
			default:
				break;
			}
		}


		//***构造函数***
		DirectoryInfo::DirectoryInfo(DirType DirType) :_DirType(DirType)
		{
			switch (DirType)
			{
			case cfl::file::asset:
				this->dirPath = "";
				break;
			case cfl::file::appStorage:
				this->dirPath = getAppStoragePath();
				break;
			case cfl::file::document:
				this->dirPath = getDocumentPath();
				break;
			default:
				break;
			}
		}

		FileInfoPtr DirectoryInfo::getfile(const char* filepath) const
		{
			auto info= new FileInfo(this,filepath) ;
			return std::shared_ptr<FileInfo>(info);
		}
		

		FileInfo::FileInfo(const DirectoryInfo* dir, const char* file)
			:basedirlen(strlen(dir->dirPath)), basedir(dir)
		{
			this->filepath = std::string(dir->dirPath);
			if (file != nullptr)
			{
				if (dir->_DirType != DirType::asset)
				{
					if (*file != '/' && *file != '\\')
					{
						this->filepath.push_back('/');
					}
				}
				this->filepath.append(file);
			}
		}
		/*FileInfo::~FileInfo()
		{
			this->filepath.clear();
		}*/

		bool FileInfo::isExists()
		{
			if (basedir->_DirType == DirType::asset)
			{
				//**调用asset的方法 - -蛋疼
				if (application == nullptr)
				{
					trace_e("CFLfile::_readfromAssetManager android_app not inited");
					return false;
				}
				auto asset = AAssetManager_open(application->activity->assetManager, filepath.c_str(), AASSET_MODE_BUFFER);
				if (asset != nullptr)
				{
					AAsset_close(asset);
					return true;
				}
				else
				{
					return false;
				}


			}
			else
			{
				auto code = fopen(filepath.c_str(), "r");
				if (code == nullptr)
				{
					return false;
				}
				else
				{
					fclose(code);
					return true;
				}
			}
		}

		/*
		删除文件
		*/
		bool FileInfo::deletefile()
		{
			if (basedir->_DirType == DirType::asset)
			{
				trace_e("CFLfile::deletefile asset dir is readonly");
			}
			else
			{ 
				if (remove(filepath.c_str()) == 0)
				{
					return true;
				}
				else
				{
					trace_e("CFLfile::deletefile failed %s",filepath.c_str());
					return false;
				}

			}
		}

		int FileInfo::getFileLength()
		{
			
			if (basedir->_DirType == DirType::asset)
			{
				//**调用asset的方法 - -蛋疼
				if (application == nullptr)
				{
					trace_e("CFLfile::_readfromAssetManager android_app not inited");
					return -1;
				}
				auto asset = AAssetManager_open(application->activity->assetManager, filepath.c_str(), AASSET_MODE_BUFFER);
				if (asset != nullptr)
				{
					auto len = AAsset_getLength(asset);
					AAsset_close(asset);
					return len;
				}
				else
				{
					return -1;
				}


			}
			else
			{ 
				auto fp = fopen(filepath.c_str(), "r");
				if (fp ==nullptr)
				{
					return -1;
				}
				fseek(fp, 0, SEEK_END);

				auto len = ftell(fp);
				fclose(fp);
				return len;

			}
		}

		static bool _readFile(const char* filepath,FileData* buff)
		{
			do
			{

				FILE *fp = fopen(filepath, "rb");

				if (fp == nullptr)
				{
					buff->status = FileIoStatus::fileNotFound;
					break;
				}
				if (fseek(fp, 0, SEEK_END) != 0)
				{
					buff->status = FileIoStatus::fileReadError;
					fclose(fp);
					break;
				}
				auto len = ftell(fp);
				buff->filesize = len;

				if (fseek(fp, 0, SEEK_SET) != 0)
				{
					buff->status = FileIoStatus::fileReadError;
					fclose(fp);
					break;
				}

				char* data = new char[len + 1]; /*故意多缓存一个字节，置0，应对字符串*/ data[len] = 0;
				auto readlen = fread(data, sizeof(char), len, fp);

				fclose(fp);

				buff->data = data;
				buff->length = readlen;

				if (buff->length == len)
				{
					buff->status = FileIoStatus::success;
					return true;
				}
				else
				{
					buff->status = FileIoStatus::fileReadError;
					return false;
				}
			} while (false);

			
			buff->length = 0;
			buff->data = nullptr;
			return false;
		}

		bool _readfromAssetManager(const char* filepath,FileData* buff)
		{
			if (application == nullptr)
			{
				buff->status = FileIoStatus::fileNotFound;
				trace_e("CFLfile::_readfromAssetManager android_app not inited");
				return false;
			}

			auto asset = AAssetManager_open(application->activity->assetManager, filepath, AASSET_MODE_BUFFER);
			if (asset != nullptr)
			{
				auto len = AAsset_getLength(asset);
				auto data = new char[len+1];/*故意多缓存一个字节，置0，应对字符串*/ data[len] = 0;

				auto readlen=AAsset_read(asset, data, len);
				AAsset_close(asset);

				buff->filesize = len;
				
				buff->data = data;
				buff->length = readlen;

				if (len == readlen)
				{
					buff->status = FileIoStatus::success;
					return true;
				}
				else
				{
					buff->status = FileIoStatus::fileReadError;
					return false;
				}

			}
			else
			{
				buff->status = FileIoStatus::fileNotFound;
				trace_e("CFLfile::_readfromAssetManager file not exist:%s",filepath);
				return false;
			}

		}

		bool FileInfo::readFile(FileData* buff)
		{
			buff->status = FileIoStatus::reading;
			if (basedir->_DirType == DirType::asset)
			{
			    return _readfromAssetManager(filepath.c_str(), buff);
			}
			else
			{
				//trace("begin readbuff %s", filepath.c_str());
				return _readFile(filepath.c_str(), buff);
			}

		}

		static FileData* _readfileasync(FileInfo file, FileData* buff, FileInfo::readFileProgress progress)
		{
			do
			{

				FILE *fp = fopen(file.getFilePath(), "rb");

				if (fp == nullptr)
				{
					buff->status = FileIoStatus::fileNotFound;
					break;
				}
				if (fseek(fp, 0, SEEK_END) != 0)
				{
					buff->status = FileIoStatus::fileReadError;
					fclose(fp);
					break;
				}
				auto len = ftell(fp);
				buff->filesize = len;
				buff->length = 0;
				if (fseek(fp, 0, SEEK_SET) != 0)
				{
					buff->status = FileIoStatus::fileReadError;
					fclose(fp);
					break;
				}

				char* data = new char[len+1];/*故意多缓存一个字节，置0，应对字符串*/ data[len] = 0;
				auto readpos = data;

				auto needread = len;

				while (needread>0)
				{
					auto toread = needread > CFL_FILEBUFFSIZE ? CFL_FILEBUFFSIZE : needread;
					auto readlen = fread(readpos, sizeof(char), toread, fp);
					readpos += readlen;

					buff->length += readlen;

					if (toread == readlen)
					{
						if (progress)
						{
							progress(buff);
						}
						needread -= toread;
					}
					else
					{
						break;
					}
				}


				fclose(fp);

				buff->data = data;
				
				if (buff->length == len)
				{
					buff->status = FileIoStatus::success;
					return buff;
				}
				else
				{
					buff->status = FileIoStatus::fileReadError;
					return buff;
				}
			} while (false);


			buff->length = 0;
			buff->data = nullptr;
			return buff;
		}
		
		static FileData* _readfromAssetManagerAsync(FileInfo file, FileData* buff, FileInfo::readFileProgress progress)
		{
			if (application == nullptr)
			{
				buff->status = FileIoStatus::fileNotFound;
				trace_e("CFLfile::_readfromAssetManager android_app not inited");
				return buff;
			}

			auto asset = AAssetManager_open(application->activity->assetManager, file.getFilePath(), AASSET_MODE_BUFFER);
			if (asset != nullptr)
			{
				auto len = AAsset_getLength(asset);
				auto data = new char[len + 1];/*故意多缓存一个字节，置0，应对字符串*/ data[len] = 0;
				auto readpos = data;

				buff->filesize = len;
				buff->length = 0;
				//auto readlen = AAsset_read(asset, data, len);
				
				auto needread = len;
				while (needread>0)
				{
					auto toread = needread > CFL_FILEBUFFSIZE ? CFL_FILEBUFFSIZE : needread;
					auto readlen = AAsset_read(asset, readpos, toread); //fread(data, sizeof(char), toread, fp);
					readpos += readlen;

					buff->length += readlen;

					if (toread == readlen)
					{
						if (progress)
						{
							progress(buff);
						}
						needread -= toread;
					}
					else
					{
						break;
					}
				}
				
				
				AAsset_close(asset);

				buff->data = data;
				
				if (buff->length == len)
				{
					buff->status = FileIoStatus::success;
					return buff;
				}
				else
				{
					buff->status = FileIoStatus::fileReadError;
					return buff;
				}

			}
			else
			{
				buff->status = FileIoStatus::fileNotFound;
				trace_e("CFLfile::_readfromAssetManagerAsync file not exist:%s", file.getFilePath());
				return buff;
			}
		}
		
		//asyncFunc::AsyncHandler<FileData*>
		ReadFileAsync
		FileInfo::readFileAsync(FileData* buff, readFileProgress progress)
		{
			buff->status = FileIoStatus::reading;
			if (basedir->_DirType == DirType::asset)
			{
				//return asyncFunc::beginInvoke(_readfromAssetManagerAsync, *this, buff, progress);
				
				return ReadFileAsync(
					innerType(
					asyncFunc::beginInvoke(_readfromAssetManagerAsync, *this, buff, progress)));

				//return _readfromAssetManager(filepath.c_str(), buff);
			}
			else
			{
				
				//return asyncFunc::beginInvoke(_readfileasync, (*this), buff, progress);
				return ReadFileAsync(
					innerType(
					asyncFunc::beginInvoke(_readfileasync, (*this), buff, progress)));
				
				//trace("begin readbuff %s", filepath.c_str());
				//return _readFile(filepath.c_str(), buff);
			}
		}



		size_t FileInfo::writeFile(const char* buff, size_t count)
		{
			if (basedir->_DirType == DirType::asset)
			{
				trace_e("asset cannot write");
				return 0;
			}


			char test[CFL_MAXFILEPATH] = { '\0' };
			auto begin = filepath.begin();
			auto end = filepath.end();

			

			int pos = 0;
			while (pos < basedirlen + 1)
			{
				*(test + pos) = *begin;
				pos++;
				begin++;
			}

			
			while (begin<end)
			{
				*(test + pos) = *begin;
				if (*begin == '\\' || *begin == '/')
				{
					auto dir = opendir(test);
					if (dir == nullptr)
					{
						auto mk = mkdir(test, S_IRUSR | S_IWUSR);
						if (mk < 0)
						{
							trace_e("writeFile makedir error %s", test);
							return false;

						}
					}
					else
					{
						closedir(dir);
					}

					
				}
				pos++;
				begin++;
			}

			
			FILE* wf;
			wf = fopen(test, "wb");
			if (wf ==nullptr)
			{
				trace_e("writeFile openfile error %s", test);
				return 0;
			}

			auto towritebytes = count;
			decltype(count) writed = 0;
			auto writepos = buff;

			while (towritebytes>0)
			{
				auto wc = towritebytes > CFL_FILEBUFFSIZE ? CFL_FILEBUFFSIZE : towritebytes;
				auto inputsize = fwrite(writepos, sizeof(char), wc, wf);
				writed += inputsize;
				if (inputsize != wc)
				{
					//***写入出错***
					break;
				}

				writepos += wc;
				towritebytes -= wc;
			}

			fclose(wf);
			if (writed != count)
			{
				trace_e("writeFile write count error %s", test);
				//***写入数量不对
				return writed;
			}

			LOGI("file writed %s", test);
			return writed;
	
		}
	


	}

}