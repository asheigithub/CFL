#ifndef IGAMEIMAGEPROGRAM_H
#define IGAMEIMAGEPROGRAM_H

#include "GraphicRenders.h"
#include <vector>

namespace cfl
{
	namespace graphic
	{


		//�������GameImage����ɫ��Ч��
		class IGameImageEffect
		{
		public:
			IGameImageEffect(int type) :effectType(type){};
			virtual ~IGameImageEffect() = default;

			//������������һ������
			const int effectType;

			//��ȡ��ɫ������
			//addlist: ������´�������ɫ������Ҫ�����Ž��б��
		 	virtual render::GLProgram* getProgram(Texture2DRef* refTex2D ,std::vector<render::GLProgram*> addlist )=0;
			
			//����Ƿ����һ����ɫ�������
			virtual bool checkCompatible(const IGameImageEffect* rhs) = 0;

			//��ͼʱ����������ɫ���Ĳ���
			virtual void setProgramArgs(const Graphic* graphic,
				render::GLProgram* program,
				Texture2DRef* refTex2D, const GameImage* image,void* args) = 0;
		};

		struct GameImageEffectData;

		typedef void(*GameImageEffectDX)(GameImageEffectData* data);

		struct GameImageEffectData
		{
			GameImageEffectData() :
				effect(nullptr),
				dx(nullptr)
			{}

			IGameImageEffect* effect;
			GameImageEffectDX dx;
			
			void reset()
			{
				if (dx != nullptr)
				{
					dx(this);
					dx = nullptr;
				}
			}

		};

	}
}



#endif // !IGAMEIMAGEPROGRAM_H
