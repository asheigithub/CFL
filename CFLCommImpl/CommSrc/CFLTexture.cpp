#include "Graphic/CFLTexture.h"
#include "CFLEntry.h"

#include "Content/CFLCTF.h"
#include "Content/CFLKTX.h"
#include "Content/CFLFileDataResource.h"

#include <mutex>

namespace cfl
{
	namespace render
	{
		namespace textures
		{
#define CHECK_ISNOTDISPOSE CFL_ASSERT(!_indata->isdisposed); //ֻ��δdispose����¿ɲ���

			struct indata
			{
				indata():
					internalformat( tex2d_internalformat::glrgba ),
					width(0),
					height(0),
					streaminglevels(0),
					tex2dobj(nullptr),
					autoGenLevels(false),
					default_min_filter(tex2d_filtertype::FilterType::gllinear),
					default_mag_filter(tex2d_filtertype::FilterType::gllinear),
					default_warp_s(tex2d_wrap::WarpType::glclamp),
					default_warp_t(tex2d_wrap::WarpType::glclamp)
				{}

				indata(tex2d_internalformat::InternalFormat internalformat,
					GLsizei width,
					GLsizei height,
					size_t streaminglevels,
					bool autoGenLevels
					):
					internalformat(internalformat),
					width(width),
					height(height),
					streaminglevels(streaminglevels),
					tex2dobj(nullptr),
					autoGenLevels(autoGenLevels),
					default_min_filter(tex2d_filtertype::FilterType::gllinear),
					default_mag_filter(tex2d_filtertype::FilterType::gllinear),
					default_warp_s(tex2d_wrap::WarpType::glclamp),
					default_warp_t(tex2d_wrap::WarpType::glclamp)
				{
					
				}


				tex2d_internalformat::InternalFormat internalformat;
				GLsizei width;
				GLsizei height;
				size_t streaminglevels;
				IGLObj* tex2dobj;

				tex2d_filtertype::FilterType default_min_filter;
				tex2d_filtertype::FilterType default_mag_filter;
				tex2d_wrap::WarpType default_warp_s;
				tex2d_wrap::WarpType default_warp_t;

				bool autoGenLevels;
				bool isdisposed=false;
				bool isdone = false;//����Ƿ��Ѿ�����������ɲ��ύGPU��GPU������һ����Ⱦ���ڽ��к˶ԣ��˺����֪���Ƿ���Ч
				bool isfaild = false; //����ϴ��׶����Ƿ����κδ�����
				float progress = 0;

				
				std::mutex mtx;
				std::vector< std::tuple<Texture2D,void*, void(*)(Texture2D* tex, void* args)>> progresseventlist;
				std::vector< std::tuple<Texture2D, void*, void(*)(Texture2D* tex, void* args)>> doneeventlist;


				void onProgress() //�������ȸ����¼�
				{
					std::unique_lock<std::mutex> lock(mtx);

					for (auto begin = progresseventlist.begin(); begin != progresseventlist.end() ; begin++)
					{
						std::get<2>(*begin)(&std::get<0>(*begin), std::get<1>(*begin));
					}
					progresseventlist.clear();
				}

				void onDone()	//��������¼�
				{
					std::unique_lock<std::mutex> lock(mtx);
					isdone = true;

					for (auto begin = doneeventlist.begin(); begin != doneeventlist.end(); begin++)
					{
						std::get<2>(*begin)(&std::get<0>(*begin), std::get<1>(*begin));
					}
					doneeventlist.clear();
				}

			};

			static void disposeindata(indata* _indata)
			{
				
				if (!_indata->isdisposed)
				{
					_indata->isdisposed = true;

					if (_indata->tex2dobj)
					{
						//��Ҫ���˵�������
						/*CFLContext::getCurrentInstance()
						->glObjCollection->deleteTex2D();*/
						auto obj = reinterpret_cast<GLTex2D*>(_indata->tex2dobj);
						CFLContext::getCurrentInstance()->glObjCollection->deleteTex2D(
							obj
							);

						_indata->tex2dobj = nullptr;

					}

				}
				
			}


			static void ondestory(indata* todel)
			{
				
				disposeindata(todel);
				delete todel;
			}

			Texture2D::Texture2D()
				:_indata(new indata(),ondestory)
			{
				
			}

			Texture2D::Texture2D(
				tex2d_internalformat::InternalFormat format, 
				GLsizei width, 
				GLsizei height, 
				size_t streamlevels)
				: _indata(new indata(format,width,height,streamlevels,false),ondestory)

			{
				
			}

			void Texture2D::dispose()
			{
				disposeindata(_indata.get());
			}

			
			void Texture2D::setDefaultMinFilter(tex2d_filtertype::FilterType filter)
			{
				_indata->default_min_filter = filter;
			}
			
			void Texture2D::setDefaultMagFilter(tex2d_filtertype::FilterType filter)
			{
				_indata->default_mag_filter = filter;
			}
			
			void Texture2D::setDefaultWarpS(tex2d_wrap::WarpType type)
			{
				_indata->default_warp_s = type;
			}
			
			void Texture2D::setDefaultWarpT(tex2d_wrap::WarpType type)
			{
				_indata->default_warp_t = type;
			}

			tex2d_filtertype::FilterType Texture2D::getDefaultMinFilter() const{ return _indata->default_min_filter; }
			
			tex2d_filtertype::FilterType Texture2D::getDefaultMagFilter() const{ return _indata->default_mag_filter; }
			
			tex2d_wrap::WarpType Texture2D::setDefaultWarpS() const{ return _indata->default_warp_s; };
			
			tex2d_wrap::WarpType Texture2D::setDefaultWarpT() const{ return _indata->default_warp_t; }


			bool Texture2D::getIsDone() const
			{
				return _indata->isdone;
			}

			float Texture2D::getPorgress() const
			{
				return _indata->progress;
			}

			void Texture2D::addUploadEventHandler(void(*progress)(Texture2D* tex, void* args), void(*done)(Texture2D* tex, void* args),void* args)
			{
				if (_indata->isdone)
				{
					if (progress)
					{
						progress(this, args);
					}
					if (done)
					{
						done(this, args);
					}
				}
				else
				{
					std::unique_lock<std::mutex> lock(_indata->mtx);

					if (progress)
					{
					_indata->progresseventlist.push_back(
						std::make_tuple(*this, args, progress)
						);
					}
					if (done)
					{
					_indata->doneeventlist.push_back(
						std::make_tuple(*this, args, done)
						);
					}
					
				}

			}


			static bool loadFromKTX(Texture2D tex, std::shared_ptr<indata> _indata,
				const std::shared_ptr<content::IGLDataResource> source,
				size_t offset
				)
			{
				char* data = reinterpret_cast<char*>( source->getData());
				data += offset;

				content::KTXFile ktxfile;
				auto isSuccess = content::parseKTXFile(data, source->getByteLength()-offset, &ktxfile);

				if (isSuccess)
				{
					if (ktxfile.header.numberOfArrayElements != 0 || ktxfile.header.numberOfFaces != 1)
					{
						LOGE("Texture2D ����ʹ�����������CubeMap��KTX \n");
						return false;
					}
					else if (ktxfile.header.numberOfMipmapLevels == 0)
					{
						LOGE("Texture2D mipmap���������� \n");
						return false;
					}

					_indata->width = ktxfile.header.pixelWidth;
					_indata->height = ktxfile.header.pixelHeight;
					_indata->streaminglevels = ktxfile.header.numberOfMipmapLevels - 1;
					_indata->autoGenLevels = false;
					_indata->internalformat =
						static_cast<tex2d_internalformat::InternalFormat>(
						ktxfile.header.glInternalFormat);


					auto width = _indata->width;
					auto height = _indata->height;
					for (auto begin = ktxfile.datalist.begin(); begin < ktxfile.datalist.end(); begin++)
					{

						if (ktxfile.header.glType == 0)
						{
							tex.uploadFromImageData(begin->miplevel, width, height,
								tex2d_inputformat::glcompressed,
								tex2d_pixeltype::glcompressed,
								source,
								begin->imageSize,
								begin->pos+offset,
								false
								);

						}
						else
						{
							tex.uploadFromImageData(
								begin->miplevel, width,
								height,
								static_cast<tex2d_inputformat::InputFormat>(ktxfile.header.glFormat),
								static_cast< tex2d_pixeltype::PixelType >(ktxfile.header.glType),
								source,
								4,
								begin->pos+offset,
								false
								);
						}


						width = width / 2;
						height = height / 2;

						if (width < 1){ width = 1; }
						if (height < 1){ height = 1; }
					}



					return true;
				}
				else
				{
					return false;
				}
			}


			static void whenDsloaded( Texture2D tex,std::shared_ptr<indata> _indata, 
				const std::shared_ptr<content::IGLDataResource> source,
				size_t offset, bool autoGenMipmap
				)
			{
				if (_indata->isdisposed) //���ܷ������첽���ص�������첽���ؽ������Ƕ����Ѿ�֪ͨ��dispose��
				{
					source->close();
					LOGE("when dataloaded,this texture hasdisposed\n");
					return;
				}
				
				if (!source->isValid())
				{
					LOGE("datasource read error %s\n",source->getError() );

					
					_indata->isfaild = true;
					_indata->onDone();

					return;
				}
				
				
				char* data = reinterpret_cast<char*>(source->getData());
				data += offset;
				
				bool isctf;
				{

					content::CTFHeader header;
					//memcpy(&header, data, sizeof(header));

					auto isSuccess = content::parseCTFHeader(data, source->getByteLength()-offset, &header);

					isctf= isSuccess;

					if (isSuccess)
					{
						if (header.contentType == content::CTFContentType::rgba)
						{
							_indata->width = header.width;
							_indata->height = header.height;
							_indata->streaminglevels = 0;
							_indata->autoGenLevels = autoGenMipmap;
							_indata->internalformat = tex2d_internalformat::glrgba;


							tex.uploadFromImageData(
								0, header.width,
								header.height,
								tex2d_inputformat::glrgba,
								tex2d_pixeltype::glubyte,
								source,
								header.stride,
								offset + sizeof(header),
								autoGenMipmap
								);

							return;
						}
						else if (header.contentType == content::CTFContentType::ktx)
						{
							if (loadFromKTX(tex, _indata, source, offset + sizeof(header) ))
							{
								return;
							}
						}
						else
						{
							LOGE("CTFContentType����\n");
						}
						
					}

				}

				if (!isctf)
				{
					//�Ƿ�ֱ�Ӷ�KTX
					if (loadFromKTX( tex,_indata,source,offset ))
					{
						return;
					}

				}

				{
					LOGE("��ʽ���󡣼Ȳ���CTF Ҳ����KTX\n");

					source->close();
					_indata->isfaild = true;
					_indata->onDone();

					return;

				}
			}

			void Texture2D::uploadFromDataResource(
				const std::shared_ptr<content::IGLDataResource> source,
				size_t offset, bool autoGenMipmap, bool async)
			{
				CHECK_ISNOTDISPOSE;

				CFL_ASSERT(!_indata->isdone); //��Ҫ�ظ�ʹ�ô˷����ϴ�����
				
				//��source��ȡ��Ϻ�
				

				if (!source->isDone())
				{
					if (!async) //ͬ��
					{
						
						source->load();
						whenDsloaded(*this,
							_indata,source,offset,autoGenMipmap
							);
					}
					else
					{
						//�첽
						
						auto args = new std::tuple<Texture2D,std::shared_ptr<indata>,
							std::shared_ptr<content::IGLDataResource>, size_t, bool>
							(*this,_indata, source, offset, autoGenMipmap);

						source->loadAsync([]
							(std::shared_ptr<content::IGLDataResource> ds,void * args){
								
							auto paras = reinterpret_cast < std::tuple < Texture2D,
								std::shared_ptr<indata>,
								std::shared_ptr<content::IGLDataResource>, 
								size_t, bool > * > (args);
							
							whenDsloaded(
								std::get<0>(*paras),
								std::get<1>(*paras),
								std::get<2>(*paras),
								std::get<3>(*paras),
								std::get<4>(*paras));
							
								
							delete paras;
								
							},
								[]
							(std::shared_ptr<content::IGLDataResource> ds, void * args)
							{
								auto paras = reinterpret_cast < std::tuple < Texture2D,
									std::shared_ptr<indata>,
									std::shared_ptr<content::IGLDataResource>,
									size_t, bool > * > (args);

								auto __indata = std::get<1>(*paras);// ->progress = ds->getProgress();
								__indata->progress = ds->getProgress();
								__indata->onProgress();

							}
								
								
								,args);


						

					}
				}
				else
				{
					whenDsloaded(*this,
						_indata, source, offset, autoGenMipmap
						);
				}
			}

			void Texture2D::initFromFile(const cfl::file::DirType dir, const CFLString path,
				bool autoGenMipmap, bool async )
			{
				CHECK_ISNOTDISPOSE;
				CFL_ASSERT(!_indata->isdone); //��Ҫ�ظ�ʹ�ô˷����ϴ�����

				auto fs = std::make_shared<content::FileDataResource>(
					dir, path);

				uploadFromDataResource(fs, 0, autoGenMipmap, async);

			}






			void Texture2D::uploadFromImageData(
				int mipLevel,
				GLsizei width,
				GLsizei height,
				tex2d_inputformat::InputFormat format,
				tex2d_pixeltype::PixelType type,
				std::shared_ptr<content::IGLDataResource> source,
				size_t stride,
				size_t offset,
				bool autoGenMipmap
				)
			{

				
				CHECK_ISNOTDISPOSE;
				CFL_ASSERT(!_indata->isdone); //��Ҫ�ظ�ʹ�ô˷����ϴ�����
				//����С�趨
				//CFL_ASSERT(_indata->width ==  width * pow(2,(mipLevel)) && _indata->height ==  height * pow(2,(mipLevel)));
				
				

				GLTex2D* tex2d = reinterpret_cast<GLTex2D*>(_indata->tex2dobj);

				if (_indata->tex2dobj == nullptr)
				{
					tex2d= CFLContext::getCurrentInstance()->glObjCollection->createTex2D();
					
					if (mipLevel == 0)
					{
						tex2d->default_mag_filter = _indata->default_mag_filter;
						tex2d->default_min_filter = _indata->default_min_filter;
						tex2d->default_warp_s = _indata->default_warp_s;
						tex2d->default_warp_t = _indata->default_warp_t;
					}

					_indata->tex2dobj = tex2d;


				}
				
				if (!source->isDone())
				{
					
					source->load();
				}
				
				_indata->progress = 1;

				tex2d->appendMipLevel(mipLevel, _indata->internalformat,
					width, height, format, type, source,offset, stride);
				
				if (autoGenMipmap || mipLevel == _indata->streaminglevels)
				{
					tex2d->texImage2d(autoGenMipmap);

					
					_indata->onDone();
				}
				
			}

			bool Texture2D::isValid() const
			{
				
				if (!_indata->tex2dobj)
				{
					return false;
				}
				return _indata->tex2dobj->isValid();
			}

			bool Texture2D::isFaild() const
			{
				
				if (_indata->isfaild)
				{
					return true;
				}

				if (!_indata->tex2dobj)
				{
					return false;
				}
				return dynamic_cast<GLTex2D*>(_indata->tex2dobj)->isFaild();
			}


			GLuint Texture2D::getHandle() const
			{
				CHECK_ISNOTDISPOSE;
				
				CFL_ASSERT(_indata->tex2dobj);
				if (!(_indata->tex2dobj)) //���ֻ��ϱ����쳣������һ�������ڵ���������
				{
					return 0xffff;
				}
				return _indata->tex2dobj->getHandle();
			}
		}
	}
}
