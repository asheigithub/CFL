#ifndef CFL_BUILDINDATA_H
#define CFL_BUILDINDATA_H

#include "../Graphic/IGLDataResouce.h"

namespace cfl
{
	namespace content
	{
		
		/*
		直接输入数据的资源类
		*/
		class BuildInData : public IGLDataResource
		{
		public:
			BuildInData(void* data,const size_t bytes);
			
			
			void* getData() override final { return data; };

			size_t getByteLength() override final{ return bytes; };

			bool isDone() override final;
			
			bool isValid() override final;
			
			const char* getError() override final;
			
			void load() override final{};
			
			void loadAsync(GLDataResourceLoadCallbacker callbacker, GLDataResourceLoadCallbacker progress, void* args) override final;
			
			
			float getProgress() override final{ return 1; };
			
			void close() override final {};
		private:
			void* data;
			size_t bytes;
			
		};


	}
}



#endif // !CFL_BUILDINDATA_H
