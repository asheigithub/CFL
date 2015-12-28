#ifndef _CFL_BinaryCache
#define _CFL_BinaryCache

#include "../CFLFile.h"
#include "../CFLString.h"

//#include <atomic>

namespace cfl
{
	namespace content
	{
		/*
		当文件读取操作锁死时抛出。
		使用 asyncFunc::beginInvoke 来调用BinaryCache::getData()。可能会发生死锁。
		检测到这种死锁发生时将抛出此异常
		*/
		class BinaryDataDeathLockException :public std::runtime_error
		{
		public:
			BinaryDataDeathLockException(const char* path) :runtime_error(
				"异常:不要使用 asyncFunc::beginInvoke 来调用BinaryCache::getData()。可能会发生死锁，将在死锁发生时抛出。"),
				filepath(path) 
			{
				//LOGE("%s\n",exception::what());
			}

			const char* filepath;

		};




		class BinaryCache;

		struct BinaryDataInCache; //保存在缓存里的内容


		void _fileReadProgress(file::FileData* data);
		void _dofileReadComplate(file::FileData* data);

		/*
		二进制内容
		使用引用计数管理。
		*/
		struct BinaryData
		{
			
			friend void _fileReadProgress(file::FileData* data);
			friend void _dofileReadComplate(file::FileData* data);
			

			BinaryData(BinaryDataInCache* _data);
			BinaryData(const BinaryData& rhs);
			BinaryData&
				operator=(const BinaryData& rhs);
			~BinaryData();
			
			friend class BinaryCache;
			
			//获取资源数据
			char* getData() const;// { return _fdata.data; };
			
			//资源路径
			CFLString getResoucePath() const;// { return _path; };

			//读取状态
			file::FileIoStatus getStatus() const;// { return _fdata.status; };

			//资源应该有的长度
			size_t getResourceSize() const;// { return _fdata.filesize; };

			//实际读取到的长度
			size_t getReadSize() const;// { return _fdata.length; };

			//是否是异步调用但是同步完成了。
			bool getIsSyncComplet() const{ return _issynccomplet; }
		private:
			
			BinaryDataInCache* _cachedata;
			bool _issynccomplet;
		};
		
		/*
		二进制内容缓存容器
		*/
		class BinaryCache 
		{
			friend void _dofileReadComplate(file::FileData* data);
			friend void subStructSize(BinaryDataInCache* _indata);
		public:
			static BinaryCache* getInstance();

			//同步获取文件内容
			BinaryData getData(const file::DirType basedir,const CFLString& path );

			//异步获取文件内容
			BinaryData getDataAsync(const file::DirType basedir,const CFLString& path,
									 void(*progress)(BinaryData*,void*),void(*complate)(BinaryData*,void*),void* callbackerargs);



			void collect();                     

			
			//获取已缓存的内容大小
			inline size_t getCachedSize(){ return _cache_size; }; 
			//获取最大缓存大小
			inline size_t getMaxCacheSize(){ return _max_cache_size; };

			//设置最大缓存大小
			inline void setMaxCacheSize(size_t value){ _max_cache_size = value; };

		private:
			BinaryCache();
			~BinaryCache();

			//std::atomic<size_t> _cache_size;
			size_t _cache_size;

			size_t _max_cache_size=0;

			BinaryCache(BinaryCache&) = delete;
			BinaryCache& operator=(BinaryCache&) = delete;
		};

		

	}
}

#endif // !_CFL_BinaryCache
