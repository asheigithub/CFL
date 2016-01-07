#ifndef IGAMEIMAGEPROGRAM_H
#define IGAMEIMAGEPROGRAM_H

#include "GraphicRenders.h"
#include <vector>

namespace cfl
{
	namespace graphic
	{


		//定义绘制GameImage的着色器效果
		class IGameImageEffect
		{
		public:
			IGameImageEffect(int type) :effectType(type){};
			virtual ~IGameImageEffect() = default;

			//用于区分类别的一个数字
			const int effectType;

			//获取着色器对象
			//addlist: 如果是新创建的着色器则需要把它放进列表里。
		 	virtual render::GLProgram* getProgram(Texture2DRef* refTex2D ,std::vector<render::GLProgram*> addlist )=0;
			
			//检测是否和另一个着色程序兼容
			virtual bool checkCompatible(const IGameImageEffect* rhs) = 0;

			//绘图时最终设置着色器的参数
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
