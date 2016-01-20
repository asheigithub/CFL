#include "CFLFile.h"

#include <string>
#include <stdlib.h>
#include <Windows.h>
#include <shlobj.h>

#include <direct.h>  
#include <io.h>
#include "CFLAsyncFunc.h"


namespace cfl
{
	namespace file
	{
		using namespace cfl::asyncFunc;

		//**定义返回类**
		struct ReadFileAsync::innerType : public AsyncHandler<FileData*>
		{
			ReadFileAsync::innerType(AsyncHandler<FileData*>& v) :
				AsyncHandler<FileData*>(v)
			{
				
			}
		};
		static std::allocator<ReadFileAsync> readfileasync_alloc;

		typedef void(*ReadFileCallBackFun)(ReadFileAsync&) ;

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

		/*struct ReadFileAsync
		{
			struct innerType;

			bool hasCamplate();
			void setCallbackFun(void(*callbacker)(ReadFileAsync&));
			FileData* endInvoke() const;

		private:
			std::shared_ptr<innerType> inner;
		};*/


		//***********

		std::string  DirectoryInfo::assetPath;
		std::string  DirectoryInfo::appStoragePath;
		std::string  DirectoryInfo::documentPath;
		
		//***初始化系统目录
		void initDirecionaryInfo(void* ctx)
		{

			static bool inited = false;
			if (!inited)
			{
				inited = true;

				char* progpath;
				_get_pgmptr(&progpath);

				char* filenamest=progpath;

				auto len = strlen(progpath) - 1;
				while (len >= 0)
				{
					if (*"." == *(progpath + len))
					{
						*(progpath + len) = 0;
					}
					else if (*"\\" == *(progpath + len))
					{
						filenamest = (progpath + len) + 1;
						auto c = (progpath + len);
						*c = 0;
						break;
					}
					len--;
				}

				char mydoc[1024];
				auto folder = SHGetSpecialFolderPath(NULL, mydoc, CSIDL_MYDOCUMENTS, FALSE);
				if (folder)
				{
					DirectoryInfo::documentPath = mydoc;
				}
				else
				{
					DirectoryInfo::documentPath = "";
				}
				folder = SHGetSpecialFolderPath(NULL, mydoc, CSIDL_APPDATA, FALSE);
				if (folder)
				{
					DirectoryInfo::appStoragePath = mydoc;
					DirectoryInfo::appStoragePath.append("\\");
					DirectoryInfo::appStoragePath.append(filenamest);
				}
				else
				{
					DirectoryInfo::appStoragePath = "";
				}

				DirectoryInfo::assetPath = progpath;



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
			}

			return nullptr;

		}

		//***构造函数***
		DirectoryInfo::DirectoryInfo(DirType DirType) :_DirType(DirType)
		{
			switch (DirType)
			{
			case cfl::file::asset:
				this->dirPath = getAssetPath();
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
			:basedirlen(strlen(dir->dirPath)),  basedir(dir)
		{
			
			this->filepath = std::string(dir->dirPath);
			//this->basedirlen = strlen(dir.dirPath);
			if (file != nullptr)
			{
				if (*file != '/' && *file != '\\')
				{
					this->filepath.push_back('\\');
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
			auto code = _access(filepath.c_str(), 0);
			if (code == 0)
			{
				return true;
			}
			else
			{
				return false;
			}

		}

		/*
		删除文件
		*/
		bool FileInfo::deletefile()
		{
			if (remove(filepath.c_str()) == 0)
			{
				return true;
			}
			else
			{
				return false;
			}

		}

		int FileInfo::getFileLength()
		{
			FILE *fp;
			auto ok = fopen_s(&fp, filepath.c_str(), "r");
			if (ok != 0)
			{
				return -1;
			}
			fseek(fp, 0, SEEK_END);

			auto len= ftell(fp);
			fclose(fp);
			return len;

		}

		std::shared_ptr<fileStream> FileInfo::openFileStreamForRead()
		{
			FILE *fp;
			auto ok = fopen_s(&fp, filepath.c_str(), "rb");
			if (ok != 0)
			{
				return nullptr;
			}

			return std::make_shared<fileStream>(fp,nullptr);
		}

		


		bool FileInfo::readFile(FileData* buff)
		{
			buff->status = FileIoStatus::reading;
			do
			{
				FILE *fp;
				auto ok = fopen_s(&fp, filepath.c_str(), "rb");
				if (ok != 0)
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
				buff->length = 0;
				char* data = new char[len+1]; //为了处理字符串的情况。
				data[len] = 0;

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
						needread -= toread;
					}
					else
					{
						break;
					}
				}

				//auto readlen = fread(data, sizeof(char), len, fp);

				fclose(fp);

				buff->data = data;
				//buff->length = readlen;

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



		static FileData* _readfileasync(FileInfo file, FileData* buff, FileInfo::readFileProgress progress)
		{
			
			do
			{
				FILE *fp;
				auto ok = fopen_s(&fp, file.getFilePath(), "rb");
				if (ok != 0)
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
				buff->length = 0;
				char* data = new char[len+1]; //处理字符串的情况。故意多读1个0字节
				data[len] = 0;

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

				//auto readlen = fread(data, sizeof(char), len, fp);

				fclose(fp);

				buff->data = data;
				//buff->length = readlen;

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

		//asyncFunc::AsyncHandler<FileData*>
		ReadFileAsync
		FileInfo::readFileAsync(FileData* buff, readFileProgress progress)
		{
			buff->status = FileIoStatus::reading;
			//return asyncFunc::beginInvoke(_readfileasync, (*this), buff, progress);
			//auto k = asyncFunc::beginInvoke(_readfileasync, (*this), buff, progress);

			return asyncFunc::beginInvoke(_readfileasync, (*this), buff, progress); //ReadFileAsync( ReadFileAsync::innerType( k));

		}

		size_t FileInfo::writeFile(const char* buff,size_t count)
		{
			char test[CFL_MAXFILEPATH] = { '\0' };
			
			auto begin = filepath.begin();
			auto end = filepath.end();

			int pos = 0;
			while (pos < basedirlen ) //windows需要测试目录是否创建
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
					auto code = _access(test, 0);
					if (code == EACCES || code==EINVAL)
					{
						goto errorcase;
					}
					else if (code<0)
					{
						if (_mkdir(test) < 0)
						{
							goto errorcase;
						}
					}
				}
				pos++;
				begin++;
			}
			
			FILE* wf;
			auto ok = fopen_s(&wf,test, "wb");
			if (ok !=0)
			{
				goto errorcase;
			}

			auto towritebytes = count;
			decltype(count) writed = 0;
			auto writepos = buff;

			while (towritebytes>0)
			{
				auto wc = towritebytes > CFL_FILEBUFFSIZE ? CFL_FILEBUFFSIZE : towritebytes;
				auto inputsize= fwrite(writepos, sizeof(char), wc, wf);
				writed += inputsize;
				if (inputsize != wc)
				{
					//***写入出错***
					break;
				}

				writepos += wc;
				towritebytes -= wc;
			}
			//auto writed= fwrite(buff, sizeof(char), count, wf);
			fclose(wf);
			
			return writed;
errorcase:
			
			return 0;
		}
	}

}
