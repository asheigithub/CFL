#ifndef CFL_VERTBUFFERTEMPLATE_H
#define CFL_VERTBUFFERTEMPLATE_H

#include "CFLEntry.h"

namespace cfl
{
	namespace graphic
	{

		template<typename V> 
		struct VertBufferAllocRet
		{
			VertBufferAllocRet(V* ptr, size_t offset)
				:ptr(ptr), offset(offset)
			{}

			V* ptr;
			size_t offset;
		};

		template<typename V>
		class VecDataSource;

		template<typename V>
		class VertBuffer
		{
		public:

			VertBuffer() :size(64), used(0)
			{
				data = new V[size];
				ptrpos = data;

			}

			~VertBuffer()
			{
				datasource = nullptr;
				delete[] data;
			}

			void reset()
			{
				ptrpos = data;
				used = 0;
			}

			VertBufferAllocRet<V> getVects(size_t num)
			{
				if (used + num > size) //重分配
				{
					auto nsize = size;
					while (nsize<used + num)
					{
						nsize = nsize * 2;
					}

					V* temp = new V[nsize];
					memcpy(temp, data, used * sizeof(V));

					delete[] data;

					data = temp;


					ptrpos = data;
					ptrpos += used;
					size = nsize;
				}

				auto ret = VertBufferAllocRet<V>(ptrpos, used);
				ptrpos += num;
				used += num;

				return  ret;
			}

			V* data;

			size_t getSize(){ return size; }

			size_t getUsed(){ return used; }

			std::shared_ptr<VecDataSource<V>> datasource;

		private:
			V* ptrpos;

			size_t used;
			size_t size;



		};



		template<typename V>
		class VecDataSource :public content::IGLDataResource
		{
		public:

			~VecDataSource() = default;

			VertBuffer<V>* _data;

			void setData(VertBuffer<V>* d)
			{
				_data = d;
			}


			//读取数据
			void* getData() override final
			{
				return _data->data;
			}
			//数据字节长度
			size_t getByteLength() override final
			{
				return _data->getUsed() * sizeof(V);
			}

			//是否已完成操作
			bool isDone() override final { return true; }
			//数据是否正确
			bool isValid() override final { return true; }
			//查询错误信息
			const char* getError() override final { return "\0"; }
			//同步加载资源
			void load() override final{}
			//异步加载资源
			void loadAsync(content::GLDataResourceLoadCallbacker callbacker,
				content::GLDataResourceLoadCallbacker progress, void* args) override final {};
			//获取加载进度
			float getProgress() override final{ return 1; }
			//通知可以释放资源。。不过释放之后，则下次使用需要再次读取。
			void close() override final {}
		};


	}
}


#endif // !CFL_VERTBUFFERTEMPLATE_H
