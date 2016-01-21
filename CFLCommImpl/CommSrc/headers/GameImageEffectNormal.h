#ifndef GAMEIMAGEEFFCTNORMAL_H
#define GAMEIMAGEEFFCTNORMAL_H

#include "IGameImageEffect.h"

namespace cfl
{
	namespace graphic
	{
		
		/*
		Ĭ��GameImageShader
		*/
		class GameImageEffectNormal : public IGameImageEffect
		{
		public:
			static std::shared_ptr<GameImageEffectNormal> getInstance();

			render::GLProgram* getProgram(Texture2DRef* refTex2D, std::vector<render::GLProgram**>& addlist) override final;

			//����Ƿ����һ����ɫ�������
			bool checkCompatible(const IGameImageEffect* rhs) override final;

			//��ͼʱ����������ɫ���Ĳ���
			void setProgramArgs(const Graphic* graphic,
				render::GLProgram* program,
				Texture2DRef* refTex2D, const GameImage* image) override final;

		private:
			GameImageEffectNormal() :IGameImageEffect(GAMEIMAGEEFFECT_NORMALTYPE){}


		};


	}
}



#endif // !GAMEIMAGEEFFCTNORMAL_H
