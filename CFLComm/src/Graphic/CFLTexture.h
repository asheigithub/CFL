#ifndef CFL_TEXTURE_H
#define CFL_TEXTURE_H

#include "CFLTextureBase.h"
#include "CFLGLTex2D.h"
#include "IGLDataResouce.h"

#include "../Content/CFLBinaryCache.h"

namespace cfl
{
	namespace render
	{
		namespace textures
		{
			//内部数据类
			struct indata;
			//Texture 类表示上载到渲染上下文的二维纹理。 
			class Texture2D :public TextureBase
			{
				
			public:
				


				Texture2D();
				
				Texture2D(tex2d_internalformat::InternalFormat format, GLsizei width, GLsizei height)
					:Texture2D(format,width,height,0)
				{}

				Texture2D(tex2d_internalformat::InternalFormat format,GLsizei width,GLsizei height,size_t streamlevels);

				Texture2D(bool autoGenLevels, tex2d_internalformat::InternalFormat format, GLsizei width, GLsizei height);

				//获取纹理资源是否已经载入内存，准备上传GPU.
				bool getIsDone() const;
				//获取纹理加载进度
				float getPorgress() const;

				//添加纹理事件
				void addUploadEventHandler(void(*progress)(Texture2D* tex, void* args), void(*done)(Texture2D* tex, void* args),void* args);

				//从文件中读取纹理
				void initFromFile(const cfl::file::DirType dir,const CFLString path,bool autoGenMipmap,bool async=false);

				//从二进制资源里上传纹理(格式见CTFHeader)
				void uploadFromDataResource(const std::shared_ptr<content::IGLDataResource> source,  size_t offset, bool autoGenMipmap, bool async = false);

				//直接从图像数据中上传纹理。当miplevel等于指定最大miplevel或者选择自动生成mipmap时开始上传。只有当当前是绘图线程时，才能设置immediately为true!!!
				void uploadFromImageData(
					int mipLevel,
					GLsizei width,
					GLsizei height,
					tex2d_inputformat::InputFormat format,
					tex2d_pixeltype::PixelType type,
					std::shared_ptr<content::IGLDataResource> source,
					size_t stride,
					size_t offset,//二进制数据偏移量 
					bool autoGenMipmap,
					bool immediately=false
					);

				//更新指定miplevel的图像
				void updateImageData(GLint level,
					GLsizei width, GLsizei height,
					tex2d_inputformat::InputFormat format,
					tex2d_pixeltype::PixelType type,
					std::shared_ptr<content::IGLDataResource> pixelSource,
					size_t offset,
					size_t stride,
					bool immediately=false);


				//获取内部纹理格式
				tex2d_internalformat::InternalFormat getInternalformat() const;
				
				//获取纹理宽度
				GLsizei getWidth() const;
				
				//获取纹理高度
				GLsizei getHeight() const;
				
				//获取mip级别数量
				//不过鉴于反正要么没有，要么全有，所以这个数字应该由大小决定。
				size_t getStreamingLevels() const;
				
				//获取纹理的GL句柄
				GLuint getHandle() const;

				//纹理是否已验证有效
				bool isValid() const;
				//纹理创建过程中是否有任何错误
				bool isFaild() const;

				//手工释放与此纹理关联的资源。 释放后将不能再对此纹理进行操作
				void dispose() override final;

				//设置默认的缩小过滤器
				void setDefaultMinFilter( tex2d_filtertype::FilterType filter );
				//设置默认的放大过滤器
				void setDefaultMagFilter(tex2d_filtertype::FilterType filter);
				//设置默认U方向缠绕模式
				void setDefaultWarpS(tex2d_wrap::WarpType type);
				//设置默认V方向缠绕模式
				void setDefaultWarpT(tex2d_wrap::WarpType type);

				//获取默认的缩小过滤器
				tex2d_filtertype::FilterType getDefaultMinFilter() const;
				//获取默认的放大过滤器
				tex2d_filtertype::FilterType getDefaultMagFilter() const;
				//获取默认U方向缠绕模式
				tex2d_wrap::WarpType setDefaultWarpS() const;
				//获取默认V方向缠绕模式
				tex2d_wrap::WarpType setDefaultWarpT() const;

			private:
				
				std::shared_ptr < indata > _indata;

			};

			


		}

	}

}

#endif // !CFL_TEXTURE_H
