#include "Graphic/CFLGameImage.h"

#include <mutex>
#include <atomic>

#include <memory>
#include "Content/CFLFileDataResource.h"
#include "CFLEntry.h"

namespace cfl
{
	namespace graphic
	{
		class tex2dRef_indata
		{
		public:
			tex2dRef_indata(std::shared_ptr<render::textures::Texture2D> texture2d,
							const std::shared_ptr<render::textures::Texture2D> separate,
							const file::DirType* dir,
							const CFLString* path,
							content::CTFAlphaType alphatype,
							content::CTFHasAlpha  hasalpha
							) :
				refcount(0),
				texture2d(texture2d),
				separate(separate),
				dir(dir),
				path(path),

				alphatype(alphatype),
				hasalpha(hasalpha),

				lastrefcounter(0),
				__useinbatchCounter(0),
				__useinbatchStIdx(0)

			{
				hasupload = false;
				metaloaded = false;
				isMetaFailed = false;
			}

			~tex2dRef_indata()
			{
				if (dir)
				{
					delete dir;
					dir = nullptr;
				}
				if (path)
				{
					delete path;
					path = nullptr;
				}

				

			}

			size_t refcount;
			std::mutex mtx;
			std::shared_ptr<render::textures::Texture2D> texture2d;
			std::shared_ptr<render::textures::Texture2D> separate;


			const file::DirType* dir;
			const CFLString* path;

			content::CTFAlphaType alphatype;
			content::CTFHasAlpha  hasalpha;
			
			std::atomic<bool> hasupload;
			
			std::atomic<bool> metaloaded;
			std::atomic<bool> isMetaFailed;

			size_t lastrefcounter;


			int __useinbatchCounter;
			int __useinbatchStIdx;

		};

		bool Texture2DRef::equals(const Texture2DRef* rhs) const
		{
			if (rhs == nullptr)
			{
				return false;
			}

			return texdata->texture2d.get() == rhs->texdata->texture2d.get();
		}

		void Texture2DRef::setLastRefCounter(size_t frame)
		{
			texdata->lastrefcounter = frame;
		}

		int Texture2DRef::__useinbatchCounter() const
		{
			return texdata->__useinbatchCounter;
		}
		void Texture2DRef::__useinbatchCounter(int v)
		{
			texdata->__useinbatchCounter = v;
		}

		int Texture2DRef::__useinbatchStIdx() const
		{
			return texdata->__useinbatchStIdx;
		}
		void Texture2DRef::__useinbatchStIdx(int v)
		{
			texdata->__useinbatchStIdx = v;
		}

		Texture2DRef::Texture2DRef(const std::shared_ptr<render::textures::Texture2D> texture2d,
			const std::shared_ptr<render::textures::Texture2D> separate,
			content::CTFAlphaType alphatype,
			content::CTFHasAlpha  hasalpha
			):
			texdata(new tex2dRef_indata( texture2d,separate,nullptr,nullptr , alphatype,hasalpha ))
		{
			texdata->hasupload = true;
			texdata->metaloaded = true;
			texdata->isMetaFailed = false;
		}
		Texture2DRef::Texture2DRef(const file::DirType dir, const CFLString& path
			):
			texdata( new tex2dRef_indata( 
						nullptr , 
						nullptr ,
						new file::DirType(dir), 
						new CFLString(path), content::CTFAlphaType::alphachannal,content::CTFHasAlpha::notransparent )	
			 )
		{
			
		}

		Texture2DRef::~Texture2DRef()
		{

		}

		

		content::CTFAlphaType Texture2DRef::getAlphaType() const
		{
			return texdata->alphatype;
		}

		content::CTFHasAlpha  Texture2DRef::getHasAlpha() const
		{
			return texdata->hasalpha;
		}

		bool Texture2DRef::isValid() const
		{
			if (!texdata->metaloaded.load(std::memory_order_relaxed))
			{
				return false;
			}
			std::unique_lock<std::mutex> lock(texdata->mtx);
			if (texdata->texture2d)
			{
				return texdata->texture2d->isValid();
			}
			return false;
		}

		bool Texture2DRef::isFailed() const
		{
			if (texdata->isMetaFailed.load(std::memory_order_relaxed))
			{
				return false;
			}

			std::unique_lock<std::mutex> lock(texdata->mtx);

			if (texdata->separate)
			{
				if (texdata->separate->isFaild())
				{
					return true;
				}
			}

			if (texdata->texture2d)
			{
				return texdata->texture2d->isFaild();
			}
			return false;
		}

		bool Texture2DRef::isDone() const
		{
			if (!texdata->metaloaded.load(std::memory_order_relaxed))
			{
				return false;
			}
			std::unique_lock<std::mutex> lock(texdata->mtx);

			if (texdata->separate)
			{
				if (!texdata->separate->getIsDone())
				{
					return false;
				}
			}

			if (texdata->texture2d)
			{
				return texdata->texture2d->getIsDone();
			}
			return false;
		}


		static void ctffile_loaded(std::shared_ptr<tex2dRef_indata> texdata, 
			std::shared_ptr<content::IGLDataResource> fs,
			bool isasync)
		{
			if (!fs->isValid())
			{
				texdata->isMetaFailed = true;
				texdata->metaloaded = false;
			}
			else
			{
				texdata->isMetaFailed = false;
				texdata->metaloaded = true;
			}

			content::CTFHeader ctfheader;
			if (content::parseCTFHeader( reinterpret_cast<char*>( fs->getData()), fs->getByteLength(), &ctfheader))
			{
				texdata->alphatype = ctfheader.alphaType;
				texdata->hasalpha = ctfheader.hasAlpha;

				if (ctfheader.contentType == content::CTFContentType::ktx)
				{
					texdata->texture2d->uploadFromDataResource(fs, sizeof(ctfheader), false, isasync);

					if (texdata->alphatype == content::CTFAlphaType::separate)
					{
						//需要追加alpha纹理
						texdata->separate.reset(new render::textures::Texture2D());

						texdata->separate->uploadFromDataResource(fs, sizeof(ctfheader) + ctfheader.length, false, isasync);

					}


				}
				else
				{
					texdata->texture2d->uploadFromDataResource(fs, 0, false, isasync);
				}
			}
			else
			{
				texdata->alphatype = content::CTFAlphaType::alphachannal;
				texdata->hasalpha = content::CTFHasAlpha::notransparent;

				texdata->texture2d->uploadFromDataResource(fs, 0, false, isasync);
			}
			
		}

		void Texture2DRef::upload(bool async)
		{
			if (!texdata->hasupload)
			{
				if (texdata->dir == nullptr)
				{
					throw std::runtime_error("非法操作,路径没有初始化");
				}
				

				std::unique_lock<std::mutex> lock(texdata->mtx);
				if (!texdata->hasupload)
				{
					texdata->texture2d.reset(new render::textures::Texture2D());
					//texdata->texture2d->initFromFile(*texdata->dir, *texdata->path, false, async);
					
					auto fs = std::make_shared<content::FileDataResource>(
						*texdata->dir, *texdata->path);

					if (async) //异步
					{
						texdata->hasupload = true;

						//auto args = new std::shared_ptr<tex2dRef_indata>(texdata);
						//auto argfileds = new std::shared_ptr<content::FileDataResource>(fs);

						std::tuple<std::shared_ptr<tex2dRef_indata>, std::shared_ptr<content::FileDataResource>>*
							packedArgs =
							new  std::tuple<std::shared_ptr<tex2dRef_indata>, 
								std::shared_ptr<content::FileDataResource>>
								(
									texdata,fs
								);

						fs->loadAsync(
						[]
						(std::shared_ptr<content::IGLDataResource> ds, void * args)
						{
							
							/*std::shared_ptr<tex2dRef_indata>* arg =
								reinterpret_cast<std::shared_ptr<tex2dRef_indata>*>(args);*/
							std::tuple < std::shared_ptr<tex2dRef_indata>,
								std::shared_ptr < content::FileDataResource >>* arg
								= reinterpret_cast<std::tuple < std::shared_ptr<tex2dRef_indata>,
								std::shared_ptr<content::FileDataResource >> *>(args);

							
							//auto ptr = *arg; //( std::get<0>(*arg) );
							auto ptr = (std::get<0>(*arg));
							
							if (!ptr->hasupload)
							{
								LOGI("当文件加载完成时，引用已结束");
								return;
							}
							
							ctffile_loaded(ptr, ds, true);
							
							delete arg;
							
						},
							nullptr,packedArgs);
					}
					else
					{
						//同步
						texdata->hasupload = true;

						fs->load();
						ctffile_loaded(texdata, fs, false);
					}


					
					
					
					
				}
			}
		}

		void Texture2DRef::addOneRef()
		{
			std::unique_lock<std::mutex> lock(texdata->mtx);
			++texdata->refcount;
		}
		void Texture2DRef::minusOneRef()
		{
			std::unique_lock<std::mutex> lock(texdata->mtx);
			--texdata->refcount;
			if (texdata->refcount == 0)
			{
				if (texdata->texture2d)
				{
					if (!texdata->texture2d->isFaild())
					{
						texdata->texture2d->dispose();
						texdata->texture2d.reset();
					}
					else
					{
						//读取错误的纹理，不用反复处理
					}
					
				}
				
				if (texdata->separate)
				{
					if (!texdata->separate->isFaild())
					{
						texdata->separate->dispose();
						texdata->separate.reset();
					}
					else
					{
						//读取错误的纹理，不用反复处理
					}
				}

				texdata->hasupload = false;
			}

		}

		render::textures::Texture2D* Texture2DRef::getTexture2D() const
		{
			std::unique_lock<std::mutex> lock(texdata->mtx);
			return texdata->texture2d.get();
		}

		render::textures::Texture2D* Texture2DRef::getSeparateTexture() const
		{
			std::unique_lock<std::mutex> lock(texdata->mtx);
			return texdata->separate.get();
		}

	}
}