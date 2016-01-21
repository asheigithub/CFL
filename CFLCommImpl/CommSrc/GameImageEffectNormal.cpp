#include "headers/GameImageEffectNormal.h"
#include "CFLEntry.h"

#include "buildinshaders/GameImage_TinyColor_Shader.h"
#include "buildinshaders/GameImage_Separate_TinyColor_Shader.h"
#include "buildinshaders/GameImage_Atlas_TinyColor.h"


namespace cfl
{
	namespace graphic
	{
		std::shared_ptr<GameImageEffectNormal> GameImageEffectNormal::getInstance()
		{
			//static GameImageEffectNormal instance;
			static std::shared_ptr<GameImageEffectNormal> instance(new GameImageEffectNormal());
			return instance;
		}

		

		//着色器(rgba+tinycolor)
		static render::GLProgram* gameimage_tinycolor=nullptr;
		//着色器(另一张透明纹理图+tinycolor)
		static render::GLProgram* gameimage_tinycolor_separate = nullptr;
		//着色器(atlas透明纹理)
		static render::GLProgram* gameimage_tinycolor_atlas = nullptr;

		


		static render::GLProgram* build(const char* vert, const char* frag, render::GLProgram*& progloc, std::vector<render::GLProgram**>& addlist)
		{
			if (progloc == nullptr)
			{
				LOGI("created!!");
				auto vs = CFLContext::getCurrentInstance()->glObjCollection->createShader();
				vs->compile(vert, render::ShaderType::vertex_shader);

				auto fs = CFLContext::getCurrentInstance()->glObjCollection->createShader();
				fs->compile(frag, render::ShaderType::fragment_shader);

				progloc = CFLContext::getCurrentInstance()->glObjCollection->createProgram();
				progloc->link(vs, fs, true);

				addlist.push_back(&progloc);

			}
			return progloc;
		}

		static render::GLProgram* build_tinycolor(std::vector<render::GLProgram**>& addlist)
		{
			return build( gameimage_tinycolor_vert, gameimage_tinycolor_frag,
				gameimage_tinycolor,addlist);
		}

		static render::GLProgram* build_atlas_tinycolor(std::vector<render::GLProgram**>& addlist)
		{
			return build( gameimage_atlas_tinycolor_vert, gameimage_atlas_tinycolor_frag,
				gameimage_tinycolor_atlas, addlist);
		}

		static render::GLProgram* build_separate_tinycolor(std::vector<render::GLProgram**>& addlist)
		{
			return build( gameimage_separate_tinycolor_vert, gameimage_separate_tinycolor_frag,
				gameimage_tinycolor_separate, addlist);

		}




		render::GLProgram* GameImageEffectNormal::getProgram(
			Texture2DRef* refTex2D, std::vector<render::GLProgram**>& addlist)
		{
			auto alphatype = refTex2D->getAlphaType();

			switch (alphatype)
			{
			case cfl::content::alphachannal:
				return build_tinycolor(addlist); //grender->gameimage_tinycolor;
				break;
			case cfl::content::atlas:
				return build_atlas_tinycolor(addlist); //grender->gameimage_tinycolor_atlas;
				break;
			case cfl::content::separate:
				return build_separate_tinycolor(addlist); //grender->gameimage_tinycolor_separate;
				break;
			default:
				return build_tinycolor(addlist);  //grender->gameimage_tinycolor;
				break;
			}
		}


		//检测是否和另一个着色程序兼容
		bool GameImageEffectNormal::checkCompatible(const IGameImageEffect* rhs)
		{
			if (effectType != rhs->effectType)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		void GameImageEffectNormal::setProgramArgs(const Graphic* graphic,
			render::GLProgram* program,
			Texture2DRef* refTex2D, const GameImage* image)
		{
			if (program == gameimage_tinycolor_separate)
			{
				auto alphaLoc = program->getUniform("a_texture")->location;
				//绑alpha纹理//
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, refTex2D->getSeparateTexture()->getHandle());
				glUniform1i(alphaLoc, 1);
			}
		}
	}
}
