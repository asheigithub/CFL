#ifndef CFL_GLTEX2D_H
#define CFL_GLTEX2D_H

#include "CFLGLObj.h"
#include "IGLDataResouce.h"
#include <memory>
#include <vector>
#include <GLES2/gl2ext.h>

namespace cfl
{
	namespace render
	{
		namespace tex2d_internalformat
		{
			//纹理存储的内部格式
			enum InternalFormat  
			{
				glrgba = GL_RGBA,
				glrgb = GL_RGB,
				glluminance_alpha = GL_LUMINANCE_ALPHA,
				glluminace = GL_LUMINANCE,
				glalpha = GL_ALPHA,

				gletc1_rgb8_oes = GL_ETC1_RGB8_OES,
				glpvr1_rgba_4bpp = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG

			};
		}

		namespace tex2d_inputformat
		{
			//输入的纹理格式
			enum InputFormat
			{
				glrgba = GL_RGBA,
				glrgb = GL_RGB,
				glluminance_alpha = GL_LUMINANCE_ALPHA,
				glluminace = GL_LUMINANCE,
				glalpha = GL_ALPHA,

				glcompressed =0

			};
		}

		namespace tex2d_pixeltype
		{
			//输入的像素类型
			enum PixelType
			{
				glubyte= GL_UNSIGNED_BYTE,
				glushort_4444= GL_UNSIGNED_SHORT_4_4_4_4,

				glushort_55551= GL_UNSIGNED_SHORT_5_5_5_1,
				glushort_565= GL_UNSIGNED_SHORT_5_6_5,

				glcompressed =0
			};
		}

		namespace tex2d_filtertype
		{
			//纹理过滤类型
			enum FilterType
			{
				glnearest= GL_NEAREST,//贴图最近的采样点将使用做贴图坐标.
				gllinear = GL_LINEAR, //双线性（四个点平均）采样点作为贴图坐标.
				
				glnearest_mipmap_nearest= GL_NEAREST_MIPMAP_NEAREST,//从所选的最近的mip级别取得单点样本
				glnearest_mipmap_linear= GL_NEAREST_MIPMAP_LINEAR,  //两个最接近mip 取得样本，并在样本间插值

				gllinear_mipmap_nearest= GL_LINEAR_MIPMAP_NEAREST,  //从所选的最近mip级别中获得双线性样本
				gllinear_mipmap_linear= GL_LINEAR_MIPMAP_LINEAR     //从两个最近的mip级别中获得双线性样本，然后双线性插值，即所三线性过滤
			};
		}
		
		namespace tex2d_wrap
		{
			enum WarpType
			{
				glrepeat= GL_REPEAT, //重复
				glclamp = GL_CLAMP_TO_EDGE, //截取边缘
				glmirroredrepeat= GL_MIRRORED_REPEAT //镜像重复
			};
		}


		/*
		使用glTexImage2D创建一个2D纹理
		*/
		class GLTex2D : public IGLObj
		{
			typedef std::tuple < GLint,
				tex2d_internalformat::InternalFormat,
				GLsizei,
				GLsizei,
				tex2d_inputformat::InputFormat,
				tex2d_pixeltype::PixelType,
				std::shared_ptr<content::IGLDataResource >,
				size_t,
				size_t> Tex2DMipLevel;

			friend class GLObjCollection;

		public:
			//追加一个mip等级的纹理数据
			void appendMipLevel(GLint level, 
				tex2d_internalformat::InternalFormat internalformat,
				GLsizei width,
				GLsizei height,
				tex2d_inputformat::InputFormat format,
				tex2d_pixeltype::PixelType type,
				std::shared_ptr<content::IGLDataResource> pixelSource, 
				size_t offset,
				size_t stride);

			//执行纹理创建工作。可指定是否自动创建纹理级别。必须指定了0级，才能自动创建其余所有级别。
			void texImage2d(bool autoGenMipmap=false);

			//更新纹理数据
			void texUpdateImage2d(GLint level,
				GLsizei width,GLsizei height,
				tex2d_inputformat::InputFormat format,
				tex2d_pixeltype::PixelType type,
				std::shared_ptr<content::IGLDataResource> pixelSource,
				size_t offset,
				size_t stride);


			inline bool isValid() override final{ return _isvalid; };
			inline bool isFaild() const { return _isfaild; }

			//当纹理成功创建后，返回此纹理占用的内存字节数
			inline size_t getImageSize() const { return _imageSize; }
			//获取GLTex2D占用的内存总字节数
			static size_t getTotalImageSize();

			void close() override final;

			tex2d_filtertype::FilterType default_min_filter;//默认缩小过滤器
			tex2d_filtertype::FilterType default_mag_filter;//默认放大过滤器，放大过滤器只能是gllinear或者glnearest。

			tex2d_wrap::WarpType default_warp_s;//默认U方向缠绕模式
			tex2d_wrap::WarpType default_warp_t;//默认V方向缠绕模式

		private:
			GLTex2D(unsigned int id, std::list<IGLObj*>::iterator iter);
			~GLTex2D();

			void doTexImage2d();

			void doTexUpdateImage2d(GLint level);

			void onLostEGLContext(CFLContext* context) override final;

			void onGetNewEGLContext(CFLContext* context) override final;
			bool _isvalid;
			bool _isfaild;
			std::vector<Tex2DMipLevel>* mipmaps;
			
			bool _autogenmipmap;

			size_t _imageSize;
		};

	}
}



#endif // !CFL_GLTEX2D_H
