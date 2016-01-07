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
			//����洢���ڲ���ʽ
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
			//����������ʽ
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
			//�������������
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
			//�����������
			enum FilterType
			{
				glnearest= GL_NEAREST,//��ͼ����Ĳ����㽫ʹ������ͼ����.
				gllinear = GL_LINEAR, //˫���ԣ��ĸ���ƽ������������Ϊ��ͼ����.
				
				glnearest_mipmap_nearest= GL_NEAREST_MIPMAP_NEAREST,//����ѡ�������mip����ȡ�õ�������
				glnearest_mipmap_linear= GL_NEAREST_MIPMAP_LINEAR,  //������ӽ�mip ȡ�������������������ֵ

				gllinear_mipmap_nearest= GL_LINEAR_MIPMAP_NEAREST,  //����ѡ�����mip�����л��˫��������
				gllinear_mipmap_linear= GL_LINEAR_MIPMAP_LINEAR     //�����������mip�����л��˫����������Ȼ��˫���Բ�ֵ�����������Թ���
			};
		}
		
		namespace tex2d_wrap
		{
			enum WarpType
			{
				glrepeat= GL_REPEAT, //�ظ�
				glclamp = GL_CLAMP_TO_EDGE, //��ȡ��Ե
				glmirroredrepeat= GL_MIRRORED_REPEAT //�����ظ�
			};
		}


		/*
		ʹ��glTexImage2D����һ��2D����
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
			//׷��һ��mip�ȼ�����������
			void appendMipLevel(GLint level, 
				tex2d_internalformat::InternalFormat internalformat,
				GLsizei width,
				GLsizei height,
				tex2d_inputformat::InputFormat format,
				tex2d_pixeltype::PixelType type,
				std::shared_ptr<content::IGLDataResource> pixelSource, 
				size_t offset,
				size_t stride);

			//ִ����������������ָ���Ƿ��Զ����������𡣱���ָ����0���������Զ������������м���
			void texImage2d(bool autoGenMipmap=false);

			//������������
			void texUpdateImage2d(GLint level,
				GLsizei width,GLsizei height,
				tex2d_inputformat::InputFormat format,
				tex2d_pixeltype::PixelType type,
				std::shared_ptr<content::IGLDataResource> pixelSource,
				size_t offset,
				size_t stride);


			inline bool isValid() override final{ return _isvalid; };
			inline bool isFaild() const { return _isfaild; }

			//������ɹ������󣬷��ش�����ռ�õ��ڴ��ֽ���
			inline size_t getImageSize() const { return _imageSize; }
			//��ȡGLTex2Dռ�õ��ڴ����ֽ���
			static size_t getTotalImageSize();

			void close() override final;

			tex2d_filtertype::FilterType default_min_filter;//Ĭ����С������
			tex2d_filtertype::FilterType default_mag_filter;//Ĭ�ϷŴ���������Ŵ������ֻ����gllinear����glnearest��

			tex2d_wrap::WarpType default_warp_s;//Ĭ��U�������ģʽ
			tex2d_wrap::WarpType default_warp_t;//Ĭ��V�������ģʽ

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
