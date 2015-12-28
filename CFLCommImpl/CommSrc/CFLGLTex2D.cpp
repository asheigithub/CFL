#include "Graphic/CFLGLTex2D.h"
#include "CFLEntry.h"


namespace cfl
{
	namespace render
	{
		using namespace content;


		static size_t getSizeByPixelType(size_t width, size_t height, tex2d_inputformat::InputFormat format,size_t imgsize)
		{
			
			auto s = width*height;

			switch (format)
			{
			case cfl::render::tex2d_inputformat::glrgba:
				return s * 4;
				break;
			case cfl::render::tex2d_inputformat::glrgb:
				return s * 3;
				break;
			case cfl::render::tex2d_inputformat::glluminance_alpha:
				return s;
				break;
			case cfl::render::tex2d_inputformat::glluminace:
				return s;
				break;
			case cfl::render::tex2d_inputformat::glalpha:
				return s;
				break;
			case cfl::render::tex2d_inputformat::glcompressed:
				return imgsize;
				break;
			default:
				return s * 4;
				break;
			}


		}

		static size_t totalSize = 0;
		size_t GLTex2D::getTotalImageSize()
		{
			return totalSize;
		}

		GLTex2D::GLTex2D(unsigned int id, std::list<IGLObj*>::iterator iter) :IGLObj(id, iter),
			mipmaps(new std::vector<Tex2DMipLevel>()),
			default_min_filter(tex2d_filtertype::FilterType::gllinear),
			default_mag_filter(tex2d_filtertype::FilterType::gllinear),
			default_warp_s(tex2d_wrap::WarpType::glclamp),
			default_warp_t(tex2d_wrap::WarpType::glclamp),

			_autogenmipmap(false),
			_isfaild(false),
			_imageSize(0)
			{};

		GLTex2D::~GLTex2D()
		{
			
			close();
			
			if (mipmaps)
			{
				mipmaps->clear();
				delete mipmaps;
				mipmaps = nullptr;
			}
			
		};

		void GLTex2D::close()
		{
			if (_isvalid)
			{
				_isvalid = false;
				_isfaild = false;

				totalSize -= _imageSize;
				_imageSize = 0;

				glDeleteTextures(1, &glhandle);
				
				LOGI("Tex2D(%d) deleted!\n", objId);

			}
		}

		void GLTex2D::appendMipLevel(GLint level,
			tex2d_internalformat::InternalFormat internalformat,
			GLsizei width,
			GLsizei height,
			tex2d_inputformat::InputFormat format,
			tex2d_pixeltype::PixelType type,
			std::shared_ptr<IGLDataResource> pixelSource,
			size_t offset,
			size_t stride)
		{
			mipmaps->push_back(std::make_tuple(level, internalformat, width, height, format, type, pixelSource,offset, stride));
		}

		void GLTex2D::doTexImage2d()
		{
			CFL_ASSERT(mipmaps->size() > 0);

			GLuint textureId;
			glGenTextures(1, &textureId);
			CFL_CHECKGLERROR;

			// Bind the texture object
			glBindTexture(GL_TEXTURE_2D, textureId);

			CFL_CHECKGLERROR;

			for (auto i = mipmaps->begin() ; i != mipmaps->end()	; i++)
			{
				auto p = *i;
				GLint level = std::get<0>(p);
				tex2d_internalformat::InternalFormat internalformat = std::get<1>(p);
				GLsizei width = std::get<2>(p);
				GLsizei height = std::get<3>(p);
				tex2d_inputformat::InputFormat format = std::get<4>(p);
				tex2d_pixeltype::PixelType type = std::get<5>(p);
				std::shared_ptr<IGLDataResource> pixelSource = std::get<6>(p);
				size_t offset = std::get<7>(p);
				size_t stride = std::get<8>(p);

				//LOGI("pixelSource");

				if (!pixelSource->isDone())
				{
					//LOGI("pixelSource doload (%d)\n",objId);
					pixelSource->load();
					//LOGI("pixelSource loaded (%d)\n", objId);
				}
				//CFL_ASSERT(pixelSource->isValid());

				//LOGI("pixelSource loaded");

				if (!pixelSource->isValid())
				{
					LOGE( "upload texture error, datasource error: %s\n", pixelSource->getError() );
					_isfaild = true;
					glDeleteTextures(1, &textureId);
					return;
				}

				//LOGI("pixelSource valided");

				if (format != tex2d_inputformat::glcompressed)
				{
					if (stride % 4 == 0)
					{
						glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					}
					else if (stride % 2 == 0)
					{
						glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
					}
					else
					{
						glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
					}
					CFL_CHECKGLERROR;

					auto data = reinterpret_cast<unsigned char*>(pixelSource->getData()) + offset;

					glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, 0, format, type,
						data);
				}
				else
				{
					auto data = reinterpret_cast<unsigned char*>(pixelSource->getData()) + offset;

					auto imageSize = stride; //压缩纹理，借用stride为imageSize。

					glCompressedTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, 0, 
						imageSize, data);

				}
				
				
				pixelSource->close();

				CFL_IFGLERROR(
					_isfaild = true;
					glDeleteTextures(1, &textureId);
					return;
					)


				//加上内存大小..在压缩纹理中stride就是imageSize。而非压缩纹理则是算出来的
				_imageSize += getSizeByPixelType(width, height, format , stride);
				totalSize += _imageSize;
			}
			if (_autogenmipmap)
			{
				glGenerateMipmap(GL_TEXTURE_2D);
				CFL_CHECKGLERROR;

				size_t added = static_cast<size_t>( _imageSize *  0.35); //大体上mipmap会增加0.35
				_imageSize += added;
				totalSize += added;
			}
			
			//设置默认滤波
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER ,default_min_filter);
			CFL_CHECKGLERROR;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, default_mag_filter);
			CFL_CHECKGLERROR;

			//设置环绕方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, default_warp_s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, default_warp_t);

			
			_isfaild = false;
			_isvalid = true;
			glhandle = textureId;

		}


		void GLTex2D::texImage2d(bool autoGenMipmap )
		{
			_autogenmipmap = autoGenMipmap;

			auto fun = [this](){
				//LOGI("befor build tex %d\n", objId);
				doTexImage2d();
				//LOGI("end build tex %d\n", objId);
			};

			auto command = new funcDrawCommand< decltype(fun)>(fun,"doTexImage2d");
			pushMainLooperCommand(command);

			//LOGI("send build tex %d\n",objId);

		}

		void GLTex2D::onLostEGLContext(CFLContext* context)
		{
			close();
		}
		void GLTex2D::onGetNewEGLContext(CFLContext* context)
		{
			if (!_isvalid)
			{
				//LOGI("befor glTex2D(%d) repair\n", objId);

				doTexImage2d();
				CFL_ASSERT(_isvalid); //要求修复成功
				//LOGI("glTex2D(%d) repaired!\n", objId);
			}



		}


	}
}