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

			//����Ƿ����һ����ɫ�������
			bool checkCompatible(const IGameImageEffect* rhs) override final;

			//��ͼʱ����������ɫ���Ĳ���
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
