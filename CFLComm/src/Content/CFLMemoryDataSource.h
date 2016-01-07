#ifndef CFL_MEMORYDATASOURCE_H
#define CFL_MEMORYDATASOURCE_H


#include "../Graphic/IGLDataResouce.h"
#include <memory>
namespace cfl
{
	namespace content
	{

		/*
		使用智能指针管理一个数组作为资源
		*/
		class MemoryDataSource : public IGLDataResource
		{
		public:
			MemoryDataSource(std::shared_ptr<char> data, const size_t bytes);


			void* getData() override final { return data.get(); };

			size_t getByteLength() override final{ return bytes; };

			bool isDone() override final;

			bool isValid() override final;

			const char* getError() override final;

			void load() override final{};

			void loadAsync(GLDataResourceLoadCallbacker callbacker, GLDataResourceLoadCallbacker progress, void* args) override final;


			float getProgress() override final{ return 1; };

			void close() override final {};
		private:
			std::shared_ptr<char> data;
			size_t bytes;

		};

	}
}


#endif // !CFL_MEMORYDATASOURCE_H
