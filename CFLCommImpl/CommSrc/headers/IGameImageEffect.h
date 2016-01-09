#ifndef IGAMEIMAGEPROGRAM_H
#define IGAMEIMAGEPROGRAM_H

#include "GraphicRenders.h"
#include <vector>

namespace cfl
{
	namespace graphic
	{
		static const int GAMEIMAGEEFFECT_NORMALTYPE = 0;
		static const int GAMEIMAGEEFFECT_SDFTYPE = 1;


		//�������GameImage����ɫ��Ч��
		class IGameImageEffect
		{
		public:
			
			virtual ~IGameImageEffect() = default;

			//������������һ������
			const int effectType;

			//��ȡ��ɫ������
			//addlist: ������´�������ɫ������Ҫ�����Ž��б��
		 	virtual render::GLProgram* getProgram(Texture2DRef* refTex2D ,std::vector<render::GLProgram**>& addlist )=0;
			
			//����Ƿ����һ����ɫ�������
			virtual bool checkCompatible(const IGameImageEffect* rhs) = 0;

			//��ͼʱ����������ɫ���Ĳ���
			virtual void setProgramArgs(const Graphic* graphic,
				render::GLProgram* program,
				Texture2DRef* refTex2D, const GameImage* image) = 0;

		protected:
			IGameImageEffect(int type) :effectType(type){};

		};

		struct GameImageEffectData;

		typedef void(*GameImageEffectDX)(IGameImageEffect* effect);

		struct GameImageEffectData
		{
			GameImageEffectData() :
				effect(nullptr),
				dx(nullptr)
			{}

			IGameImageEffect* effect;
			GameImageEffectDX dx;
			
			

		};

	}
}



#endif // !IGAMEIMAGEPROGRAM_H
