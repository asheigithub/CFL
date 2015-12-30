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
				if (used + num > size) //�ط���
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


			//��ȡ����
			void* getData() override final
			{
				return _data->data;
			}
			//�����ֽڳ���
			size_t getByteLength() override final
			{
				return _data->getUsed() * sizeof(V);
			}

			//�Ƿ�����ɲ���
			bool isDone() override final { return true; }
			//�����Ƿ���ȷ
			bool isValid() override final { return true; }
			//��ѯ������Ϣ
			const char* getError() override final { return "\0"; }
			//ͬ��������Դ
			void load() override final{}
			//�첽������Դ
			void loadAsync(content::GLDataResourceLoadCallbacker callbacker,
				content::GLDataResourceLoadCallbacker progress, void* args) override final {};
			//��ȡ���ؽ���
			float getProgress() override final{ return 1; }
			//֪ͨ�����ͷ���Դ���������ͷ�֮�����´�ʹ����Ҫ�ٴζ�ȡ��
			void close() override final {}
		};


	}
}


#endif // !CFL_VERTBUFFERTEMPLATE_H
