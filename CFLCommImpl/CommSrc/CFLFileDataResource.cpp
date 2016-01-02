#include "Content/CFLFileDataResource.h"
#include "CFLEntry.h"

namespace cfl
{
	namespace content
	{
		using namespace render;

		FileDataResource::FileDataResource(const file::DirType basedir, const CFLString path)
			:basedir(basedir), 
			path(path), 
			
			_isdone(new bool(false)), 
			_isvalid(new bool(false)),
			_progress(new float(0.0f)),
			error(new CFLString("") ), 
			_isreading(new bool(false)),

			data(new BinaryDataPtr(nullptr), 
			[](BinaryDataPtr* ptr)
			{
					delete *ptr;
			}
			
			)
		{
			
		}

		void FileDataResource::ondataloaded()
		{
			CFL_ASSERT((*data) != nullptr);

			if (*data == nullptr) //�������쳣���������
			{
				*_progress = 0;
				*_isvalid = false;
			}
			else
			{
				if ((*data)->getStatus() != file::FileIoStatus::success)
				{
					*error = "readfile failed: " + (*data)->getResoucePath();
					//error->append((*data)->getResoucePath());
					*_isvalid = false;
				}
				else
				{
					//error->clear();
					*error = CFLString::empty;

					*_isvalid = true;
					*_progress = 1;
				}
			}
			*_isdone = true;
			*_isreading = false;
		}

		void FileDataResource::load()
		{
			if (*_isvalid==true)
			{
				return;
			}

			CFL_ASSERT(!(*_isreading));

			*_isreading = true;


			if (*data != nullptr)
			{
				delete *data;
				*data = nullptr;
			}

			
			try
			{
				
				*data = new BinaryData( std::move( BinaryCache::getInstance()->getData(basedir, path)));
			}
			catch (content::BinaryDataDeathLockException ex)
			{
				
				LOGI("err: %s\n", ex.what());
				*error = ex.what();

				
			}

			
			
			ondataloaded();
			
			
		}

		
		void progress(content::BinaryData* data,void* args)
		{
			
			std::tuple<
				GLDataResourceLoadCallbacker, 
				GLDataResourceLoadCallbacker,
				std::shared_ptr<FileDataResource>,
				void*>* packargs =
				reinterpret_cast<std::tuple<
				GLDataResourceLoadCallbacker, 
				GLDataResourceLoadCallbacker,
				std::shared_ptr<FileDataResource>,
				void*>*>(args);

			auto datasource = std::get<2>(*packargs);
			
			*(datasource->_progress) = (float)(data->getReadSize()) / data->getResourceSize();
			
			GLDataResourceLoadCallbacker progress = std::get<1>(*packargs);

			if (progress)
			{
				progress(datasource, std::get<3>(*packargs));
			}

		}

		void complate(content::BinaryData* data,void* args)
		{
			
			std::tuple<
				GLDataResourceLoadCallbacker, 
				GLDataResourceLoadCallbacker,
				std::shared_ptr<FileDataResource>, 
				void*>* packargs =
				reinterpret_cast<std::tuple<
				GLDataResourceLoadCallbacker, 
				GLDataResourceLoadCallbacker,
				std::shared_ptr<FileDataResource>, 
				void*>*>(args);
			
			auto datasource = std::get<2>(*packargs);

			

			*datasource->_progress = (float)(data->getReadSize()) / data->getResourceSize();

			
			if (!data->getIsSyncComplet())
			{
				datasource->ondataloaded();

				GLDataResourceLoadCallbacker callbackfun = std::get<0>(*packargs);

				if (callbackfun)
				{
					callbackfun(datasource, std::get<3>(*packargs));
				}

			}

			

			delete packargs;
			
		}

		void FileDataResource::loadAsync(GLDataResourceLoadCallbacker callbacker, GLDataResourceLoadCallbacker progresscallbacker, void* args)
		{
			
			if (*_isvalid==true)
			{
				if (progresscallbacker)
				{ 
					progresscallbacker(std::make_shared<FileDataResource>(*this), args);
				}

				if (callbacker)
				{
					callbacker(std::make_shared<FileDataResource>(*this),args);
				}
				
				return;
			}
			

			CFL_ASSERT(!(*_isreading));

			*_isreading = true;

			std::tuple<
				GLDataResourceLoadCallbacker,
				GLDataResourceLoadCallbacker,
				std::shared_ptr<FileDataResource>, void*>* packargs

				= new std::tuple<GLDataResourceLoadCallbacker, 
								GLDataResourceLoadCallbacker,
								std::shared_ptr<FileDataResource>,
								void*>(callbacker,progresscallbacker, std::make_shared<FileDataResource>(*this), args);

			
			if (*data !=nullptr)
			{
				delete *data;
				*data = nullptr;
			}
			
			
			*data = new BinaryData( 
				std::move( 
					BinaryCache::getInstance()->
					getDataAsync(
								basedir, path, progress, complate,
								packargs
						)
				
					)
				)
				;
			
			
			if ((*data)->getIsSyncComplet()) //�����ͬ���������
			{
				
				ondataloaded();

				if (progresscallbacker)
				{
					progresscallbacker(std::make_shared<FileDataResource>(*this), args);
				}
				if (callbacker)
				{
					callbacker(std::make_shared<FileDataResource>(*this), args);
				}

				
			}
			
			
		}


		void* FileDataResource::getData()
		{
			CFL_ASSERT(*_isdone==true);
			return (*data)->getData();
		}

		size_t FileDataResource::getByteLength()
		{
			CFL_ASSERT(*_isdone==true);
			return (*data)->getResourceSize();
		}

		void FileDataResource::close()
		{
			if (*data != nullptr)
			{
				delete *data;
				*data = nullptr;
			}
			*_isdone = false;
			*_isvalid = false;
		}

	}
}
