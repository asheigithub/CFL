#ifndef CFL_FILEDATARESOURCE_H
#define CFL_FILEDATARESOURCE_H


#include "../Graphic/IGLDataResouce.h"
#include "CFLBinaryCache.h"

namespace cfl
{
	namespace content
	{
		
		/*
		从文件中读取数据的资源类
		*/
		class FileDataResource : public IGLDataResource
		{
			friend void progress(content::BinaryData* data, void* args);
			friend void complate(content::BinaryData* data, void* args);

		public:
			FileDataResource(const file::DirType basedir,const CFLString path);


			void* getData() override final ;

			size_t getByteLength() override final;

			bool isDone() override final{ return *_isdone; };

			bool isValid() override final 
			{ 
				return *_isvalid; 
			};

			const char* getError() override final{ return error->c_str(); };

			void load() override final;

			void loadAsync(GLDataResourceLoadCallbacker callbacker, GLDataResourceLoadCallbacker progress, void* args) override final;


			float getProgress() override final { return *_progress; }

			void close() override final;

		private:
			const file::DirType basedir;
			const CFLString path;

			std::shared_ptr<bool> _isdone;
			std::shared_ptr<bool> _isvalid;
			std::shared_ptr<bool> _isreading;

			std::shared_ptr<float> _progress;

			typedef BinaryData* BinaryDataPtr;

			std::shared_ptr<BinaryDataPtr> data;

			std::shared_ptr<CFLString> error;

			void ondataloaded();

			

		};

	}
}



#endif // !CFL_FILEDATARESOURCE_H
