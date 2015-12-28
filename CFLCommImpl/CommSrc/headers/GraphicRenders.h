#ifndef _CFL_IGRAPHICRENDER_H
#define _CFL_IGRAPHICRENDER_H

//��Graphic�ڲ�ʹ�õĻ�����

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
			//֪ͨ��Ⱦ���м�������
			virtual void swapRenderQueue(CFLContext* ctx) = 0;
			//��Ⱦ��������Ϊ��ǰ��Ⱦ��
			virtual void onActive(CFLContext* ctx) = 0;
			//��ǰ��Ⱦ���Ӽ����Ϊ������
			virtual void deActive(CFLContext* ctx) = 0;

			const RenderType type;

			const Graphic* graphic;

		protected:
			render::RenderThreadLooper* const renderLooper;
		};



	}

}






#endif // !_CFL_IGRAPHICRENDER_H
