#ifndef CFL_LINERENDER_H
#define CFL_LINERENDER_H

#include "GraphicRenders.h"

namespace cfl
{

	namespace graphic
	{

		//画线器接口
		class IGLLineRender : public IGraphicRender
		{
		public:
			IGLLineRender(RenderType type, render::RenderThreadLooper* looper,Graphic* g) :IGraphicRender(type, looper,g)
			{};
			virtual ~IGLLineRender() = default;

			virtual void appendLine(const geom::Vector2& stpos, const geom::Vector2& edpos, const Color& color,
				float linewidth) = 0;

			virtual void drawLine(const geom::Vector2& stpos, const geom::Vector2& edpos,
				const Color& color, float linewidth) = 0;


			union linewidthrange
			{
				struct
				{
					GLfloat min;
					GLfloat max;
				};

				GLfloat range[2];
			} lineWidthRange;

		};


		//保存上次画线状态
		struct lineDrawState
		{
			lineDrawState() :drawNumsPtr(nullptr), line_width(1){}

			size_t* drawNumsPtr; //指向上次批次的顶点数的指针
			float  line_width;   //上次线宽
		};

		class lineVertBuffer;
		//float顶点格式画线器
		class GLLineRender : public IGLLineRender
		{
		public:
			GLLineRender(render::RenderThreadLooper* looper,Graphic* g);
			~GLLineRender();


			void appendLine(const geom::Vector2& stpos, const geom::Vector2& edpos,
				const Color& color, float linewidth) override final;

			void drawLine(const geom::Vector2& stpos, const geom::Vector2& edpos,
				const Color& color, float linewidth) override final;

			void onActive(CFLContext* ctx) override final{}//渲染器被激活为当前渲染器

			void deActive(CFLContext* ctx) override final{}//当前渲染器从激活变为不激活


			void onGLinited(CFLContext* ctx) override final;

			void buildShader(CFLContext* ctx);

			void swapRenderQueue(CFLContext* ctx) override final;//通知渲染队列交换

			lineVertBuffer* buff;//顶点数据缓存

			cfl::render::GLProgram* progDrawLine;

			render::GLVBO* vbo;			 //绑定的VBO

			lineDrawState* lastState;
		};



		//半精度顶点缓存
		class halfFloatlineVertBuffer;
		//halffloat顶点格式画线器
		class HalfFloatGLLineRender : public IGLLineRender
		{
		public:
			HalfFloatGLLineRender(render::RenderThreadLooper* looper, Graphic* g);
			~HalfFloatGLLineRender();


			void appendLine(const geom::Vector2& stpos, const geom::Vector2& edpos,
				const Color& color, float linewidth) override final;

			void drawLine(const geom::Vector2& stpos, const geom::Vector2& edpos,
				const Color& color, float linewidth) override final;

			void onActive(CFLContext* ctx) override final{}//渲染器被激活为当前渲染器

			void deActive(CFLContext* ctx) override final{}//当前渲染器从激活变为不激活


			void onGLinited(CFLContext* ctx) override final;

			void buildShader(CFLContext* ctx);

			void swapRenderQueue(CFLContext* ctx) override final;//通知渲染队列交换

			halfFloatlineVertBuffer* buff;//顶点数据缓存

			cfl::render::GLProgram* progDrawLine;

			render::GLVBO* vbo;			 //绑定的VBO

			lineDrawState* lastState;
		};


	}
}




#endif // !CFL_LINERENDER_H
