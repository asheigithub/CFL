#ifndef CFLINPUT_H
#define CFLINPUT_H

#include "CFLTouch.h"

namespace cfl
{
	namespace input
	{
		class Input
		{
		public:
			//当前所在像素坐标的鼠标位置。GUI坐标系左上角为(0,0)
			static cfl::geom::Vector2 mousePositon();

			/*
			当指定的鼠标按钮被按下时返回true
			button值设定为 0对应左键 ， 1对应右键 ， 2对应中键。
			*/
			static bool getMouseButton(unsigned int button);

			/*
			在用户按下指定鼠标按键的那一帧返回true。
			你需要在每帧更新中调用这个方法，此后每一帧重置状态时，
			它将不会返回true除非用户释放这个鼠标按钮然后重新按下它。
			按钮值设定为 0对应左键 ， 1对应右键 ， 2对应中键。
			*/
			static bool getMouseButtonDown(unsigned int button);
			/*
			在用户释放指定鼠标按键的那一帧返回true。
			你需要在每帧更新中调用这个方法，此后每一帧重置状态时，
			它将不会返回true除非用户释放这个鼠标按钮然后重新按下它。
			按钮值设定为 0对应左键 ， 1对应右键 ， 2对应中键。
			*/
			static bool getMouseButtonUp(unsigned int button);

		};

		
	}
}




#endif // !CFLINPUT_H
