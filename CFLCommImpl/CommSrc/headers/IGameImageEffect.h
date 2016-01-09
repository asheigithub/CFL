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


		//定义绘制GameImage的着色器效果
		class IGameImageEffect
		{
		public:
			
			virtual ~IGameImageEffect() = default;

			//用于区分类别的一个数字
			const int effectType;

			//获取着色器对象
			//addlist: 如果是新创建的着色器则需要把它放进列表里。
		 	virtual render::GLProgram* getProgram(Texture2DRef* refTex2D ,std::vector<render::GLProgram**>& addlist )=0;
			
			//检测是否和另一个着色程序兼容
			virtual bool checkCompatible(const IGameImageEffect* rhs) = 0;

			//绘图时最终设置着色器的参数
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
