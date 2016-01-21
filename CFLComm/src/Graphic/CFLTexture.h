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
			//�ڲ�������
			struct indata;
			//Texture ���ʾ���ص���Ⱦ�����ĵĶ�ά���� 
			class Texture2D :public TextureBase
			{
				
			public:
				


				Texture2D();
				
				Texture2D(tex2d_internalformat::InternalFormat format, GLsizei width, GLsizei height)
					:Texture2D(format,width,height,0)
				{}

				Texture2D(tex2d_internalformat::InternalFormat format,GLsizei width,GLsizei height,size_t streamlevels);

				Texture2D(bool autoGenLevels, tex2d_internalformat::InternalFormat format, GLsizei width, GLsizei height);

				//��ȡ������Դ�Ƿ��Ѿ������ڴ棬׼���ϴ�GPU.
				bool getIsDone() const;
				//��ȡ������ؽ���
				float getPorgress() const;

				//��������¼�
				void addUploadEventHandler(void(*progress)(Texture2D* tex, void* args), void(*done)(Texture2D* tex, void* args),void* args);

				//���ļ��ж�ȡ����
				void initFromFile(const cfl::file::DirType dir,const CFLString path,bool autoGenMipmap,bool async=false);

				//�Ӷ�������Դ���ϴ�����(��ʽ��CTFHeader)
				void uploadFromDataResource(const std::shared_ptr<content::IGLDataResource> source,  size_t offset, bool autoGenMipmap, bool async = false);

				//ֱ�Ӵ�ͼ���������ϴ�������miplevel����ָ�����miplevel����ѡ���Զ�����mipmapʱ��ʼ�ϴ���ֻ�е���ǰ�ǻ�ͼ�߳�ʱ����������immediatelyΪtrue!!!
				void uploadFromImageData(
					int mipLevel,
					GLsizei width,
					GLsizei height,
					tex2d_inputformat::InputFormat format,
					tex2d_pixeltype::PixelType type,
					std::shared_ptr<content::IGLDataResource> source,
					size_t stride,
					size_t offset,//����������ƫ���� 
					bool autoGenMipmap,
					bool immediately=false
					);

				//����ָ��miplevel��ͼ��
				void updateImageData(GLint level,
					GLsizei width, GLsizei height,
					tex2d_inputformat::InputFormat format,
					tex2d_pixeltype::PixelType type,
					std::shared_ptr<content::IGLDataResource> pixelSource,
					size_t offset,
					size_t stride,
					bool immediately=false);


				//��ȡ�ڲ������ʽ
				tex2d_internalformat::InternalFormat getInternalformat() const;
				
				//��ȡ������
				GLsizei getWidth() const;
				
				//��ȡ����߶�
				GLsizei getHeight() const;
				
				//��ȡmip��������
				//�������ڷ���Ҫôû�У�Ҫôȫ�У������������Ӧ���ɴ�С������
				size_t getStreamingLevels() const;
				
				//��ȡ�����GL���
				GLuint getHandle() const;

				//�����Ƿ�����֤��Ч
				bool isValid() const;
				//�������������Ƿ����κδ���
				bool isFaild() const;

				//�ֹ��ͷ���������������Դ�� �ͷź󽫲����ٶԴ�������в���
				void dispose() override final;

				//����Ĭ�ϵ���С������
				void setDefaultMinFilter( tex2d_filtertype::FilterType filter );
				//����Ĭ�ϵķŴ������
				void setDefaultMagFilter(tex2d_filtertype::FilterType filter);
				//����Ĭ��U�������ģʽ
				void setDefaultWarpS(tex2d_wrap::WarpType type);
				//����Ĭ��V�������ģʽ
				void setDefaultWarpT(tex2d_wrap::WarpType type);

				//��ȡĬ�ϵ���С������
				tex2d_filtertype::FilterType getDefaultMinFilter() const;
				//��ȡĬ�ϵķŴ������
				tex2d_filtertype::FilterType getDefaultMagFilter() const;
				//��ȡĬ��U�������ģʽ
				tex2d_wrap::WarpType setDefaultWarpS() const;
				//��ȡĬ��V�������ģʽ
				tex2d_wrap::WarpType setDefaultWarpT() const;

			private:
				
				std::shared_ptr < indata > _indata;

			};

			


		}

	}

}

#endif // !CFL_TEXTURE_H
