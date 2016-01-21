#ifndef GAMEIMAGEEFFECTSDF_H
#define GAMEIMAGEEFFECTSDF_H


#include "IGameImageEffect.h"


namespace cfl
{
	namespace graphic
	{
		

		class GameImageEffectSDF : public IGameImageEffect
		{
		public:
			static std::shared_ptr<GameImageEffectSDF> getInstance(float outsize);

			render::GLProgram* getProgram(Texture2DRef* refTex2D, std::vector<render::GLProgram**>& addlist) override final;

			//检测是否和另一个着色程序兼容
			bool checkCompatible(const IGameImageEffect* rhs) override final;

			//绘图时最终设置着色器的参数
			void setProgramArgs(const Graphic* graphic,
				render::GLProgram* program,
				Texture2DRef* refTex2D, const GameImage* image) override final;

		private:
			GameImageEffectSDF(float outsize) :IGameImageEffect(GAMEIMAGEEFFECT_SDFTYPE), outsize(outsize){}

			float outsize;
		};

	}
}



#endif // !GAMEIMAGEEFFECTSDF_H
