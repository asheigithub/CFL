#include "Content/CFLBinaryCache.h"
#include <atomic>
#include <vector>
#include <condition_variable>
#include "CFLEntry.h"
#include <algorithm>
#include "CFLAsyncFunc.h"
#include "CFLFile.h"

namespace cfl
{
	namespace content
	{
		struct extFileData : public file::FileData
		{
			void * tag;
		};
		
		
		struct BinaryDataInCache
		{
			
			BinaryDataInCache( const CFLString& path,const int ref)
				:path(path), ref(ref), state(0)
			{}
			
			long last_requrieframe; //最后一次请求时的帧

			extFileData fdata;

			CFLString path;
			
			int ref;

			std::recursive_mutex  _mtx; //用于获取状态的互斥量

			std::mutex _mtx_waitcomplate; //用于等待读取完成的互斥量
			std::condition_variable _cond_readcomplate;
			std::atomic<int> state;  //状态:0 :未开始读 1:读取中 2:读取完成
		};

		static void onRefClear(BinaryDataInCache* _indata);

		using  GetDataEventHandler = void(*)(BinaryData*,void*);

		struct GetDataArgs
		{
			GetDataArgs(BinaryDataInCache* indata) :binData(indata){}
			
			BinaryData binData;

			
			std::vector<std::tuple< GetDataEventHandler,void*>> progressList;
			std::vector<std::tuple<GetDataEventHandler,void*>> complateList;

		};

		BinaryData::~BinaryData()
		{
			std::unique_lock<std::recursive_mutex> lock(_cachedata->_mtx);
			if ((--_cachedata->ref) == 0)
			{
				onRefClear(_cachedata);
			}
		}

		static size_t dataidseed=0;

		BinaryData::BinaryData(const BinaryData& rhs) :_cachedata(rhs._cachedata),
			_issynccomplet(rhs._issynccomplet)
		{
			std::unique_lock<std::recursive_mutex> lock(_cachedata->_mtx);
			++(_cachedata->ref);
		}
		
		BinaryData&
			BinaryData::operator=(const BinaryData& rhs)
		{
			if (rhs._cachedata)
			{
				std::unique_lock<std::recursive_mutex> lock(rhs._cachedata->_mtx);
				++(rhs._cachedata->ref);    
			}
			
			if (_cachedata)
			{
				std::unique_lock<std::recursive_mutex> lock(_cachedata->_mtx);
				if ((--_cachedata->ref) == 0)
				{
					onRefClear(_cachedata);
				}
			}
			
			_issynccomplet = rhs._issynccomplet;
			_cachedata = rhs._cachedata;
			return *this;
		}

		BinaryData::BinaryData(BinaryDataInCache* _data) :_cachedata(_data), _issynccomplet(false)
		{
			if (_cachedata)
			{
				std::unique_lock<std::recursive_mutex> lock(_data->_mtx);
				++(_data->ref);
			}
			
		};

		char* BinaryData::getData() const { return _cachedata->fdata.data; };

		CFLString BinaryData::getResoucePath() const { return _cachedata->path; };

		file::FileIoStatus BinaryData::getStatus() const { return _cachedata->fdata.status; };

		size_t BinaryData::getResourceSize() const { return _cachedata->fdata.filesize; };

		size_t BinaryData::getReadSize() const { return _cachedata->fdata.length; };


		void subStructSize(BinaryDataInCache* _indata)
		{
			if (_indata->fdata.status == file::FileIoStatus::success)
			{
				BinaryCache::getInstance()->_cache_size -= _indata->fdata.filesize;
			}
		}


		BinaryCache* BinaryCache::getInstance()
		{
			static BinaryCache ins;
			return &ins;
		}

		static std::unordered_map < CFLString, BinaryDataInCache*, string_hasher>*
			dict;
		static std::mutex* mtx_dict;

		static std::allocator<BinaryDataInCache>* BinaryData_alloc;

		static std::deque<BinaryDataInCache*>* toremoveList; //删除队列
		static std::mutex* mtx_removeList;


		void clearToRemoveList()//清理移除队列中的内容。
		{
			
			std::unique_lock<std::mutex> lock(*mtx_removeList,std::defer_lock);

			if (!lock.try_lock())
			{
				return;
			}

			bool found = false;

			do
			{
				found = false;

				auto end = toremoveList->end();
				for (auto begin = toremoveList->begin(); begin != end; begin++)
				{
					auto data = *begin;
					CFL_ASSERT(data->ref == 0);
					//if (data->ref == 0)
					std::unique_lock<std::recursive_mutex> trylock(data->_mtx, std::defer_lock);
					if (trylock.try_lock())
					{
						trylock.unlock();

						data->fdata.close();
						BinaryData_alloc->destroy(data);
						BinaryData_alloc->deallocate(data, 1);

						toremoveList->erase(begin);
						found = true;
						break;
					}

					
				}

			} while (found);

			if (toremoveList->size() > 0)
			{
				asyncFunc::beginInvoke( clearToRemoveList);
			}

		}

		void clearDict(bool haslock)//清理标记要删除的内容
		{
			
			auto cache = BinaryCache::getInstance();
			if (cache->getCachedSize() <= cache->getMaxCacheSize())
			{
				return;
			}


			std::unique_lock<std::mutex> table_lock(*mtx_dict,std::defer_lock);
			if (!haslock)
			{
				if (!table_lock.try_lock())
				{
					asyncFunc::beginInvoke(clearDict, false);
					return;
				}
			}
			
			static std::vector<decltype(dict->begin())> norefItems;

			auto end = dict->end();
			for (auto begin = dict->begin(); begin != end;begin++)
			{
				auto data = begin->second;
				if (data->ref == 0)
				{
					norefItems.emplace_back(begin);
				}
			}

			
			std::sort(norefItems.begin(), norefItems.end(), 
				[](const decltype(dict->begin())& v1,const decltype(dict->begin())& v2 )
				{
					return v1->second->last_requrieframe < v2->second->last_requrieframe;
				});

			size_t toremoved = 0;

			
			for (auto begin = norefItems.begin(); begin != norefItems.end(); begin++)
			{
				auto data = (*begin)->second;
				std::unique_lock<std::recursive_mutex> row_lock(data->_mtx, std::defer_lock);
				if (row_lock.try_lock())
				{
					//if (data->_waitdelete)
					if (data->ref == 0)
					{
						if (cache->getCachedSize() <= cache->getMaxCacheSize())
						{
							break;
						}

						
						dict->erase(*begin);
						subStructSize(data);

						std::unique_lock<std::mutex> lock(*mtx_removeList);
						toremoveList->push_back(data);
						lock.unlock();

						toremoved++;

						

					}
				}
				
			}

			norefItems.clear();

			if (toremoved > 0)
			{
				asyncFunc::beginInvoke(clearToRemoveList);
			}
			
		}

		/*void removefromDict(BinaryDataInCache* _indata)
		{
			CFL_ASSERT(_indata->ref == 0);
			auto size = dict.erase(_indata->path);
			CFL_ASSERT(size == 1);

			subStructSize(_indata);


			std::unique_lock<std::mutex> lock(mtx_removeList);
			toremoveList.push_back(_indata);
			lock.unlock();

			asyncFunc::beginInvoke(clearToRemoveList);
		}*/

		static void onRefClear(BinaryDataInCache* _indata)
		{
			auto cache = BinaryCache::getInstance();
			if (cache->getMaxCacheSize() < cache->getCachedSize())
			{
				//尝试升级为表级别锁
				std::unique_lock<std::mutex> lock(*mtx_dict, std::defer_lock);
				if (lock.try_lock())
				{
					clearDict(true);
					lock.unlock();
				}
				else
				{
					asyncFunc::beginInvoke(clearDict,false);
				}
			}
			

			//_indata->_waitdelete = true;

			////尝试升级为表级别锁
			//std::unique_lock<std::mutex> lock(mtx_dict, std::defer_lock);
			//if (lock.try_lock())
			//{
			//	removefromDict(_indata);
			//	lock.unlock();
			//}
			//else
			//{
			//	asyncFunc::beginInvoke(clearDict);
			//}
			

		}


		BinaryCache::BinaryCache()
		{
			
			dict = new std::unordered_map < CFLString, BinaryDataInCache*, string_hasher>();
			mtx_dict=new std::mutex() ;

			BinaryData_alloc = new std::allocator<BinaryDataInCache>();

			toremoveList = new std::deque<BinaryDataInCache*>(); //删除队列
			mtx_removeList = new std::mutex();
		}
		BinaryCache::~BinaryCache()
		{
			delete dict;
			delete mtx_dict;
			delete BinaryData_alloc;
			delete toremoveList;
			delete mtx_removeList;
		}

		static CFLString getKey(const file::DirType& basedir, const  CFLString& path)
		{
			switch (basedir)
			{
			case cfl::file::asset:
				return (CFLString("f://asset/") + path);
				break;
			case cfl::file::appStorage:
				return (CFLString("f://appStorage/") + path);
				break;
			case cfl::file::document:
				return (CFLString("f://document/") + path);
				break;
			default:
				CFL_ASSERT(false); //基础目录不对
				return CFLString::empty;
				break;
			}
		}

		static BinaryData getOrCreateInData(CFLString& key)
		{
			BinaryDataInCache* indata;
			
			std::unique_lock<std::mutex> lock(*mtx_dict);
			{
				
				while (true)
				{
					auto f = dict->find(key);
					if (f != dict->end())                    //缓存中存在
					{
						indata = (f->second);

						std::unique_lock<std::recursive_mutex> lock2(indata->_mtx);

						indata->last_requrieframe = CFLContext::getCurrentInstance()->totalframes;

						//CFL_ASSERT((indata->_waitdelete == false));
						return BinaryData(indata);

						lock2.unlock();

					}
					else
					{

						indata = BinaryData_alloc->allocate(1);
						BinaryData_alloc->construct(indata, key, 0);

						auto pair = dict->emplace(std::make_pair(key, indata));
						auto iter = *pair.first;

						CFL_ASSERT(pair.second);   //已存在？？

						indata->last_requrieframe = CFLContext::getCurrentInstance()->totalframes;
						return BinaryData(indata);


					}

				} 
			}
			
		}
		
		BinaryData BinaryCache::getData(const file::DirType basedir, const  CFLString& path)
		{
			CFLString key = getKey(basedir, path);
			while (true)
			{
				
				auto hold = getOrCreateInData(key); 
			
				

				//if (!hold._cachedata)
				//{
				//	//BinaryData_alloc.destroy( hold._cachedata);
				//	//BinaryData_alloc.deallocate(hold._cachedata, 1);
				//	std::this_thread::yield();
				//	continue;
				//}


				auto indata = hold._cachedata;
			
				std::unique_lock<std::recursive_mutex > lock(indata->_mtx); 
				int state = indata->state;
			
			
				if (state == 0)
				{
					indata->state = 1; //开始读
					//追加事件参数等
					indata->fdata.tag = new GetDataArgs(indata);
					

					lock.unlock();     //解锁

					auto fh = file::getDirectory(basedir)->getfile(path.c_str())->readFile(&(indata->fdata));

					_dofileReadComplate(&(indata->fdata));

				}
				else if (state == 1)
				{
					//***读取中***
					lock.unlock();    //先解锁

					if (CFLContext::getCurrentInstance()->taskPool->checkIsPoolThread())
					{
						throw  BinaryDataDeathLockException(indata->path.c_str());
					}

					do
					{
						std::unique_lock<std::mutex> lock2(indata->_mtx_waitcomplate);
						if (indata->state != 1)
						{
							break;
						}
						auto wait= indata->_cond_readcomplate.wait_for(lock2,std::chrono::milliseconds(16));
						if (wait == std::cv_status::timeout)
						{
							if (indata->fdata.data == nullptr)
							{
								LOGE("wait timeout。");
								//throw  BinaryDataDeathLockException(indata->path.c_str());				
							}
						}

						lock2.unlock();
					} while (true);
				
				
				}
				else
				{
					//CFL_ASSERT(!(hold._cachedata->_waitdelete));
					//***已读取完成状态，什么也不做。等待退出后解锁
					
				}
				
				

				return hold;



			}
		}


		void _fileReadProgress(file::FileData* data)
		{
			auto extData = reinterpret_cast<extFileData*>(data);
			auto args = reinterpret_cast<GetDataArgs*>(extData->tag);

			std::vector<std::tuple<GetDataEventHandler,void*>> events;

			
			std::unique_lock<std::recursive_mutex > lock2(args->binData._cachedata->_mtx);

			auto progList = args->progressList;
			for (auto i: progList )
			{
				events.push_back(i);
				//i(&(args->binData));
			}
			lock2.unlock();

			for (auto i : events)
			{
				std::get<0>(i)(&(args->binData), std::get<1>(i) );
			}

		}

		void _fileReadComplate(file::ReadFileAsync& handle)//asyncFunc::AsyncHandler<file::FileData*>& handle)
		{
			auto fd = handle.endInvoke();
			_dofileReadComplate(fd);
		}
		
		 void _dofileReadComplate(file::FileData* data)
		{
			auto extData = reinterpret_cast<extFileData*>(data);
			auto args = reinterpret_cast<GetDataArgs*>(extData->tag);

			auto indata = args->binData._cachedata;

			std::vector<std::tuple<GetDataEventHandler,void*>> events;

			if (data->status == file::FileIoStatus::success) //增加已缓存的数量
			{
				BinaryCache::getInstance()->_cache_size += indata->fdata.filesize;
			}

			{//先维护状态	
				std::unique_lock<std::recursive_mutex > lock2(indata->_mtx);
				indata->state = 2;

				auto complateList = args->complateList;
				for (auto i : complateList)
				{
					events.push_back(i);
				}
				lock2.unlock();  //解锁

			}
			{//发信号
				//std::unique_lock<std::mutex > lock2(indata->_mtx_waitcomplate);
				indata->_cond_readcomplate.notify_all();              //发出读取完成信号。
				//lock2.unlock();
			}
			{
				for (auto i : events)
				{
					std::get<0>(i)(&(args->binData), std::get<1>(i) );
				}
			}

			
			delete args;
		}

		BinaryData BinaryCache::getDataAsync(const file::DirType basedir, const CFLString& path,
			GetDataEventHandler progress, GetDataEventHandler complate,void* callbackerargs)
		{
			
			while (true)
			{
				CFLString key = getKey(basedir, path);

				auto hold = getOrCreateInData(key);
				
				/*if (!hold._cachedata)
				{
					std::this_thread::yield();
					continue;
				}*/

				auto indata = hold._cachedata;

				std::unique_lock<std::recursive_mutex > lock(indata->_mtx); //锁定那个文件，进行状态操作之类
				int state = indata->state;
				if (state == 0)
				{
					indata->state = 1;
					//第一次读取时创建事件列表参数等
					indata->fdata.tag = new GetDataArgs(indata);

					auto args = reinterpret_cast<GetDataArgs*>(indata->fdata.tag);

					if (progress)
					{
						args->progressList.push_back( std::make_tuple( progress,callbackerargs));
					}
					if (complate)
					{
						args->complateList.push_back( std::make_tuple( complate,callbackerargs));
					}

					

					//***开始异步读取

					auto handle = file::getDirectory(basedir)
						->getfile(path.c_str())
						->readFileAsync(&(indata->fdata), _fileReadProgress);

					handle.setCallbackFun(_fileReadComplate);

					return hold; //BinaryData(indata);
				}
				else if (state == 1)
				{
					//在读取状态中，追加参数。
					auto args = reinterpret_cast<GetDataArgs*>(indata->fdata.tag);
					if (progress)
					{
						args->progressList.push_back( std::make_tuple( progress,callbackerargs));
					}
					if (complate)
					{
						args->complateList.push_back(std::make_tuple(complate,callbackerargs));
					}
					
					

					return hold;// BinaryData(indata);

				}
				else
				{
					//读取完成状态
					
					//auto result=BinaryData(indata);



					//CFL_ASSERT(!(hold._cachedata->_waitdelete));
					
					hold._issynccomplet = true;

					if (progress)
					{
						progress(&hold,callbackerargs);
					}
					if (complate)
					{
						complate(&hold,callbackerargs);
					}
					return  hold;// result;
				}


			}

		}


	}
}
