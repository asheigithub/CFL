#include "headers/GameImageEffectSDF.h"
#include "CFLEntry.h"
#include "buildinshaders/GameImage_SDF_Shader.h"

namespace cfl
{
	namespace graphic
	{
		
		std::shared_ptr<GameImageEffectSDF> GameImageEffectSDF::getInstance(float outsize)
		{
			auto stage = CFLContext::getCurrentInstance()->stage;

			//换算到实际像素
			auto port = stage->getGLViewPort();


			return std::shared_ptr<GameImageEffectSDF>(new GameImageEffectSDF(outsize * port.width / stage->stageWidth()));

		}

		

		//着色器(sdf)
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

		//检测是否和另一个着色程序兼容
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

			auto ss= (outsize - 16.0f) / (128.0f - 16.0f) * (8.0f-6.0f)+6.0f;
			if (ss > 18){ ss = 18; }
			
			glUniform1f(smoothrange, ss/outsize);

			return;
		}

	}
}