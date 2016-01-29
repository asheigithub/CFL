#ifndef CFLTOUCHPHASE_H
#define CFLTOUCHPHASE_H

namespace cfl
{
	namespace input
	{
		namespace touchPhase
		{
			/*
			描述手指触摸的状态。
			*/
			enum TouchPhase
			{
				/*
				手指已触摸屏幕。
				*/
				Began,
				/*
				手指在屏幕上移动。
				*/
				Moved,
				/*
				手指触摸屏幕，但并没有移动。
				*/
				Stationary,
				/*
				手指从屏幕上移开。这是一个触摸的最后状态。 
				*/
				Ended,
				/*
				系统取消跟踪触摸，如用户把屏幕放到他脸上或超过五个接触同时发生。这是一个触摸的最后状态。 
				*/
				Canceled

			};


		}

	}

}


#endif // !CFLTOUCHPHASE_H
