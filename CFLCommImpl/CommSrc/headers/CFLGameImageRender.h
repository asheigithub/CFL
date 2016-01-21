#ifndef CFL_GAMEIMAGERENDER_H
#define CFL_GAMEIMAGERENDER_H

#include "GraphicRenders.h"
#include "GameImageEffectNormal.h"

namespace cfl
{
	namespace graphic
	{
		//GameImage������
		class IGameImageRender : public IGraphicRender
		{
		public:
			IGameImageRender(RenderType type, render::RenderThreadLooper* looper,Graphic* g) :IGraphicRender(type, looper,g)
			{};
			virtual ~IGameImageRender() = default;
			//����GameImage
			virtual void drawGameImage(
				bool isAppend,//����ϴε����Ƿ�Ҳ��drawGameImage

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
			//���ۼƵĻ�ͼ��Ϣ���͸�GPU
			void callBatch();
			void buildDrawCommand(int st, int ed);
		public:
			GameImageRender(render::RenderThreadLooper* looper, Graphic* g);
			~GameImageRender();

			void onGLinited(CFLContext* ctx) override final;

			void buildShader(CFLContext* ctx);

			void swapRenderQueue(CFLContext* ctx) override final;//֪ͨ��Ⱦ���н���
			
			void onActive(CFLContext* ctx) override final{}//��Ⱦ��������Ϊ��ǰ��Ⱦ��

			void deActive(CFLContext* ctx) override final;//��ǰ��Ⱦ���Ӽ����Ϊ������

			
			void drawGameImage(
				bool isAppend,//����ϴε����Ƿ�Ҳ��drawGameImage

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


			////��ɫ��(rgba+tinycolor)
			//render::GLProgram* gameimage_tinycolor;
			////��ɫ��(��һ��͸������ͼ+tinycolor)
			//render::GLProgram* gameimage_tinycolor_separate;
			////��ɫ��(atlas͸������)
			//render::GLProgram* gameimage_tinycolor_atlas;


			render::GLVBO* vertVbo;
			render::GLVBO* indexVbo;


			std::vector<render::GLProgram**> createdPrograms;

		};

		

	}
}




#endif // !CFL_GAMEIMAGERENDER_H
