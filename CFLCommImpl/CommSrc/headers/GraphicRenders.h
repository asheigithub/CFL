#ifndef _CFL_IGRAPHICRENDER_H
#define _CFL_IGRAPHICRENDER_H

//在Graphic内部使用的绘制器

#include "Graphic/CFLGraphic.h"
#include "Graphic/CFLProgram.h"
#include "Graphic/CFLGLVBO.h"

namespace cfl
{

	namespace graphic
	{
		enum RenderType
		{
			drawline,
			gameimage,
		};


		class IGraphicRender
		{
		public:
			IGraphicRender(RenderType type, render::RenderThreadLooper* looper ,Graphic* graphic ) :type(type),
				renderLooper(looper), graphic(graphic)
			{};

			virtual ~IGraphicRender() = default;

			virtual void onGLinited(CFLContext* ctx)=0;
			//通知渲染队列即将交换
			virtual void swapRenderQueue(CFLContext* ctx) = 0;
			//渲染器被激活为当前渲染器
			virtual void onActive(CFLContext* ctx) = 0;
			//当前渲染器从激活变为不激活
			virtual void deActive(CFLContext* ctx) = 0;

			const RenderType type;

			const Graphic* graphic;

		protected:
			render::RenderThreadLooper* const renderLooper;
		};



	}

}






#endif // !_CFL_IGRAPHICRENDER_H
