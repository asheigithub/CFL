#ifndef GAMEIMAGEEFFCTNORMAL_H
#define GAMEIMAGEEFFCTNORMAL_H

#include "IGameImageEffect.h"

namespace cfl
{
	namespace graphic
	{
		
		/*
		默认GameImageShader
		*/
		class GameImageEffectNormal : public IGameImageEffect
		{
		public:
			static std::shared_ptr<GameImageEffectNormal> getInstance();

			render::GLProgram* getProgram(Texture2DRef* refTex2D, std::vector<render::GLProgram**>& addlist) override final;

			//检测是否和另一个着色程序兼容
			bool checkCompatible(const IGameImageEffect* rhs) override final;

			//绘图时最终设置着色器的参数
			void setProgramArgs(const Graphic* graphic,
				render::GLProgram* program,
				Texture2DRef* refTex2D, const GameImage* image) override final;

		private:
			GameImageEffectNormal() :IGameImageEffect(GAMEIMAGEEFFECT_NORMALTYPE){}


		};


	}
}



#endif // !GAMEIMAGEEFFCTNORMAL_H
