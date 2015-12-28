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
		���ļ���ȡ��������ʱ�׳���
		ʹ�� asyncFunc::beginInvoke ������BinaryCache::getData()�����ܻᷢ��������
		��⵽������������ʱ���׳����쳣
		*/
		class BinaryDataDeathLockException :public std::runtime_error
		{
		public:
			BinaryDataDeathLockException(const char* path) :runtime_error(
				"�쳣:��Ҫʹ�� asyncFunc::beginInvoke ������BinaryCache::getData()�����ܻᷢ��������������������ʱ�׳���"),
				filepath(path) 
			{
				//LOGE("%s\n",exception::what());
			}

			const char* filepath;

		};




		class BinaryCache;

		struct BinaryDataInCache; //�����ڻ����������


		void _fileReadProgress(file::FileData* data);
		void _dofileReadComplate(file::FileData* data);

		/*
		����������
		ʹ�����ü�������
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
			
			//��ȡ��Դ����
			char* getData() const;// { return _fdata.data; };
			
			//��Դ·��
			CFLString getResoucePath() const;// { return _path; };

			//��ȡ״̬
			file::FileIoStatus getStatus() const;// { return _fdata.status; };

			//��ԴӦ���еĳ���
			size_t getResourceSize() const;// { return _fdata.filesize; };

			//ʵ�ʶ�ȡ���ĳ���
			size_t getReadSize() const;// { return _fdata.length; };

			//�Ƿ����첽���õ���ͬ������ˡ�
			bool getIsSyncComplet() const{ return _issynccomplet; }
		private:
			
			BinaryDataInCache* _cachedata;
			bool _issynccomplet;
		};
		
		/*
		���������ݻ�������
		*/
		class BinaryCache 
		{
			friend void _dofileReadComplate(file::FileData* data);
			friend void subStructSize(BinaryDataInCache* _indata);
		public:
			static BinaryCache* getInstance();

			//ͬ����ȡ�ļ�����
			BinaryData getData(const file::DirType basedir,const CFLString& path );

			//�첽��ȡ�ļ�����
			BinaryData getDataAsync(const file::DirType basedir,const CFLString& path,
									 void(*progress)(BinaryData*,void*),void(*complate)(BinaryData*,void*),void* callbackerargs);



			void collect();                     

			
			//��ȡ�ѻ�������ݴ�С
			inline size_t getCachedSize(){ return _cache_size; }; 
			//��ȡ��󻺴��С
			inline size_t getMaxCacheSize(){ return _max_cache_size; };

			//������󻺴��С
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
