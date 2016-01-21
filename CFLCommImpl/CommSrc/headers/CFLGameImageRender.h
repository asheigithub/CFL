#ifndef CFL_GAMEIMAGERENDER_H
#define CFL_GAMEIMAGERENDER_H

#include "GraphicRenders.h"
#include "GameImageEffectNormal.h"

namespace cfl
{
	namespace graphic
	{
		//GameImage绘制器
		class IGameImageRender : public IGraphicRender
		{
		public:
			IGameImageRender(RenderType type, render::RenderThreadLooper* looper,Graphic* g) :IGraphicRender(type, looper,g)
			{};
			virtual ~IGameImageRender() = default;
			//绘制GameImage
			virtual void drawGameImage(
				bool isAppend,//标记上次调用是否也是drawGameImage

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
				BlendMode blendmode = BlendMode::alphablend,
				std::shared_ptr<IGameImageEffect> effect = GameImageEffectNormal::getInstance()
				
				)=0;

		};


		class GameImageRender : public IGameImageRender
		{
			//将累计的绘图信息推送给GPU
			void callBatch();
			void buildDrawCommand(int st, int ed);
		public:
			GameImageRender(render::RenderThreadLooper* looper, Graphic* g);
			~GameImageRender();

			void onGLinited(CFLContext* ctx) override final;

			void buildShader(CFLContext* ctx);

			void swapRenderQueue(CFLContext* ctx) override final;//通知渲染队列交换
			
			void onActive(CFLContext* ctx) override final{}//渲染器被激活为当前渲染器

			void deActive(CFLContext* ctx) override final;//当前渲染器从激活变为不激活

			
			void drawGameImage(
				bool isAppend,//标记上次调用是否也是drawGameImage

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
				BlendMode blendmode = BlendMode::alphablend,
				std::shared_ptr<IGameImageEffect> effect = GameImageEffectNormal::getInstance()
				
				) override final;


			////着色器(rgba+tinycolor)
			//render::GLProgram* gameimage_tinycolor;
			////着色器(另一张透明纹理图+tinycolor)
			//render::GLProgram* gameimage_tinycolor_separate;
			////着色器(atlas透明纹理)
			//render::GLProgram* gameimage_tinycolor_atlas;


			render::GLVBO* vertVbo;
			render::GLVBO* indexVbo;


			std::vector<render::GLProgram**> createdPrograms;

		};

		

	}
}




#endif // !CFL_GAMEIMAGERENDER_H
