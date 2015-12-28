#ifndef CFL_LINERENDER_H
#define CFL_LINERENDER_H

#include "GraphicRenders.h"

namespace cfl
{

	namespace graphic
	{

		//�������ӿ�
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


		//�����ϴλ���״̬
		struct lineDrawState
		{
			lineDrawState() :drawNumsPtr(nullptr), line_width(1){}

			size_t* drawNumsPtr; //ָ���ϴ����εĶ�������ָ��
			float  line_width;   //�ϴ��߿�
		};

		class lineVertBuffer;
		//float�����ʽ������
		class GLLineRender : public IGLLineRender
		{
		public:
			GLLineRender(render::RenderThreadLooper* looper,Graphic* g);
			~GLLineRender();


			void appendLine(const geom::Vector2& stpos, const geom::Vector2& edpos,
				const Color& color, float linewidth) override final;

			void drawLine(const geom::Vector2& stpos, const geom::Vector2& edpos,
				const Color& color, float linewidth) override final;

			void onActive(CFLContext* ctx) override final{}//��Ⱦ��������Ϊ��ǰ��Ⱦ��

			void deActive(CFLContext* ctx) override final{}//��ǰ��Ⱦ���Ӽ����Ϊ������


			void onGLinited(CFLContext* ctx) override final;

			void buildShader(CFLContext* ctx);

			void swapRenderQueue(CFLContext* ctx) override final;//֪ͨ��Ⱦ���н���

			lineVertBuffer* buff;//�������ݻ���

			cfl::render::GLProgram* progDrawLine;

			render::GLVBO* vbo;			 //�󶨵�VBO

			lineDrawState* lastState;
		};



		//�뾫�ȶ��㻺��
		class halfFloatlineVertBuffer;
		//halffloat�����ʽ������
		class HalfFloatGLLineRender : public IGLLineRender
		{
		public:
			HalfFloatGLLineRender(render::RenderThreadLooper* looper, Graphic* g);
			~HalfFloatGLLineRender();


			void appendLine(const geom::Vector2& stpos, const geom::Vector2& edpos,
				const Color& color, float linewidth) override final;

			void drawLine(const geom::Vector2& stpos, const geom::Vector2& edpos,
				const Color& color, float linewidth) override final;

			void onActive(CFLContext* ctx) override final{}//��Ⱦ��������Ϊ��ǰ��Ⱦ��

			void deActive(CFLContext* ctx) override final{}//��ǰ��Ⱦ���Ӽ����Ϊ������


			void onGLinited(CFLContext* ctx) override final;

			void buildShader(CFLContext* ctx);

			void swapRenderQueue(CFLContext* ctx) override final;//֪ͨ��Ⱦ���н���

			halfFloatlineVertBuffer* buff;//�������ݻ���

			cfl::render::GLProgram* progDrawLine;

			render::GLVBO* vbo;			 //�󶨵�VBO

			lineDrawState* lastState;
		};


	}
}




#endif // !CFL_LINERENDER_H
