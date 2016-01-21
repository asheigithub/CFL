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
		
		enum ClearMaskBit             //清屏目标的枚举
		{
			color=GL_COLOR_BUFFER_BIT,//颜色缓冲区
			depth=GL_DEPTH_BUFFER_BIT,//深度缓冲区
			stencil=GL_STENCIL_BUFFER_BIT,//模板缓冲区

		};
		typedef decltype(GL_COLOR_BUFFER_BIT) clearMaskBitValue;


		enum FilterMode		//GameImage绘图时对GameImage的过滤模式
		{
			linear,			//线性过滤
			nearest			//就近采样
		};

		enum BlendMode      //GameImage混合模式
		{
			disable,        //禁止混合。
			alphablend,		//半透明混合
			add				//颜色叠加
		};

		
		class Graphic
		{
			friend class cfl::Capability;
			friend class render::RenderThreadLooper;
		public:
			~Graphic();
			Graphic(render::RenderThreadLooper* looper);


			//当opgles初始化完成后被调用 不要手动调用此方法
			void _onGlInit(CFLContext* ctx);

			//获取GLProxy
			render::GLProxy* getGLProxy() const;


			//清理缓冲区
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


			//绘制一条直线
			void drawLine(const geom::Vector2& stpos,const geom::Vector2& edpos,const Color& color ,float linewidth=1.0f );

			struct g;
		private:
			//每帧渲染队列交换前被调用
			void onBeforRenderQueueSwap(CFLContext* ctx);

			//每帧渲染队列交换后被调用
			void onRednerQueueSwaped(CFLContext* ctx);

			void _onCapabilityInited( cfl::Capability* capablilty );

			
			g* _g;
			
		};

	}
}



#endif // !CFL_GRAPHIC_H
