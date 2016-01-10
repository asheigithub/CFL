#include "headers/GameImageEffectSDF.h"
#include "CFLEntry.h"
#include "buildinshaders/GameImage_SDF_Shader.h"

namespace cfl
{
	namespace graphic
	{
		
		GameImageEffectSDF* GameImageEffectSDF::getInstance(float outsize)
		{
			auto stage = CFLContext::getCurrentInstance()->stage;

			//���㵽ʵ������
			auto port = stage->getGLViewPort();

			return new GameImageEffectSDF(outsize * port.width / stage->stageWidth()   );
			//static GameImageEffectSDF instance;
			//return &instance;
		}

		void EffectSDFDX(IGameImageEffect* effect)
		{
			GameImageEffectSDF* sdf = reinterpret_cast<GameImageEffectSDF*>(effect);
			
			delete sdf;
			
		}

		//��ɫ��(sdf)
		static render::GLProgram* gameimage_sdf = nullptr;

		render::GLProgram* GameImageEffectSDF::getProgram(
			Texture2DRef* refTex2D, std::vector<render::GLProgram**>& addlist)
		{
			if (gameimage_sdf == nullptr)
			{
				auto vert = gameimage_sdfshader_vert;
				auto frag = gameimage_sdfshader_frag;

				auto vs = CFLContext::getCurrentInstance()->glObjCollection->createShader();
				vs->compile(vert, render::ShaderType::vertex_shader);

				auto fs = CFLContext::getCurrentInstance()->glObjCollection->createShader();
				fs->compile(frag, render::ShaderType::fragment_shader);

				gameimage_sdf = CFLContext::getCurrentInstance()->glObjCollection->createProgram();
				gameimage_sdf->link(vs, fs, true);

				addlist.push_back(&gameimage_sdf);

			}
			return gameimage_sdf;
		}

		//����Ƿ����һ����ɫ�������
		bool GameImageEffectSDF::checkCompatible(const IGameImageEffect* rhs)
		{
			if (effectType != rhs->effectType)
			{
				return false;
			}
			else
			{
				return ((int)outsize == (int)static_cast<const GameImageEffectSDF*>(rhs)->outsize);
			}
		}

		void GameImageEffectSDF::setProgramArgs(const Graphic* graphic,
			render::GLProgram* program,
			Texture2DRef* refTex2D, const GameImage* image)
		{
			auto smoothrange = program->getUniform("smoothrange")->location;

			auto ss= (outsize - 16.0f) / (128.0f - 16.0f) * (4-2.0f)+2.0f;
			if (ss > 6){ ss = 6; }

			//��alpha����//
			glUniform1f(smoothrange, ss/outsize);

			return;
		}

	}
}