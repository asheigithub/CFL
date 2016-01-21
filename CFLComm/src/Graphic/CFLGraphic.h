#ifndef CFL_GRAPHIC_H
#define CFL_GRAPHIC_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#ifdef WIN32
#include <GLES3/gl3.h>
#else
#include "gl3stub.h"
#endif

#include "../CFLDrawCommand.h"
#include "CFLProgram.h"
#include "CFLGameImage.h"

#include "CFLVertexPointer.h"
#include "CFLTexture.h"

#include "../Geom/CFLGeom.h"
#include "../Geom/CFLRectangle.h"

#include "CFLColor.h"
#include "../CFLCapability.h"
#include "CFLGLProxy.h"

#include "CFLFont.h"

namespace cfl
{
	

	namespace graphic
	{
		
		enum ClearMaskBit             //����Ŀ���ö��
		{
			color=GL_COLOR_BUFFER_BIT,//��ɫ������
			depth=GL_DEPTH_BUFFER_BIT,//��Ȼ�����
			stencil=GL_STENCIL_BUFFER_BIT,//ģ�建����

		};
		typedef decltype(GL_COLOR_BUFFER_BIT) clearMaskBitValue;


		enum FilterMode		//GameImage��ͼʱ��GameImage�Ĺ���ģʽ
		{
			linear,			//���Թ���
			nearest			//�ͽ�����
		};

		enum BlendMode      //GameImage���ģʽ
		{
			disable,        //��ֹ��ϡ�
			alphablend,		//��͸�����
			add				//��ɫ����
		};

		
		class Graphic
		{
			friend class cfl::Capability;
			friend class render::RenderThreadLooper;
		public:
			~Graphic();
			Graphic(render::RenderThreadLooper* looper);


			//��opgles��ʼ����ɺ󱻵��� ��Ҫ�ֶ����ô˷���
			void _onGlInit(CFLContext* ctx);

			//��ȡGLProxy
			render::GLProxy* getGLProxy() const;


			//��������
			void clear(float r, float g, float b, float a, float depth, int stencil, clearMaskBitValue clearmask);

			

			void drawGameImage(
				const GameImage& image,
				float tx,
				float ty,
				float opacity = 1,
				const cfl::geom::Rectangle* clip = nullptr,
				float scale = 1,
				float roation = 0,
				const Color* color = nullptr,
				FilterMode filter = FilterMode::linear,
				BlendMode blendmode = BlendMode::alphablend)
			{
				drawGameImage( std::move(image),
					tx, ty, static_cast<geom::Matrix3D*>( nullptr), opacity, clip, scale, roation, color, filter, blendmode);
			}


			void drawGameImage(
				const GameImage& image,
				float tx,
				float ty,
				const cfl::geom::Matrix* matrix,
				float opacity = 1,
				const cfl::geom::Rectangle* clip = nullptr,
				float scale = 1,
				float roation = 0,
				const Color* color = nullptr,
				FilterMode filter = FilterMode::linear,
				BlendMode blendmode = BlendMode::alphablend)
			{
				if (matrix ==nullptr)
				{ 
					drawGameImage(std::move(image),
						tx, ty, matrix, opacity, clip, scale, roation, color, filter, blendmode);
				}
				else
				{
					cfl::geom::Matrix3D m3d = cfl::geom::Matrix3D( *matrix ) ;
					drawGameImage(std::move(image),
						tx, ty,  &m3d,opacity, clip, scale, roation, color, filter, blendmode);
				}
			}
			
			void drawGameImage(
				const GameImage& image,
				float tx,
				float ty,
				const cfl::geom::Matrix3D* matrix,
				float opacity = 1,
				const cfl::geom::Rectangle* clip = nullptr,
				float scale = 1,
				float roation = 0,
				const Color* color = nullptr,
				FilterMode filter = FilterMode::linear,
				BlendMode blendmode = BlendMode::alphablend);



			void drawString(const CFLString& string, const cfl::graphic::font::Font& font, float fontsize, float x, float y, const Color* color = nullptr, const cfl::geom::Matrix3D* matrix=nullptr);


			//����һ��ֱ��
			void drawLine(const geom::Vector2& stpos,const geom::Vector2& edpos,const Color& color ,float linewidth=1.0f );

			struct g;
		private:
			//ÿ֡��Ⱦ���н���ǰ������
			void onBeforRenderQueueSwap(CFLContext* ctx);

			//ÿ֡��Ⱦ���н����󱻵���
			void onRednerQueueSwaped(CFLContext* ctx);

			void _onCapabilityInited( cfl::Capability* capablilty );

			
			g* _g;
			
		};

	}
}



#endif // !CFL_GRAPHIC_H
