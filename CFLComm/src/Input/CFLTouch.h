#ifndef CFLTOUCH_H
#define CFLTOUCH_H

#include "CFLTouchPhase.h"
#include "../Geom/CFLGeom.h"
namespace cfl
{
	namespace input
	{



		/*
		用来记录一个手指触摸在屏幕上的状态。
		*/
		struct Touch
		{
			Touch() :fingerId(0), 
				position(0,0),
				deltaPosition(0,0),
				deltaTime(0),
				tapCount(1),
				phase( touchPhase::Canceled )
			{
				
			}

			//触摸的唯一索引。
			int fingerId;
			//触摸的位置。 
			geom::Vector2 position;
			//距离上次改变的距离增量。 
			geom::Vector2 deltaPosition;
			//自上次改变的时间增量。 
			float deltaTime;
			//点击次数。 
			int tapCount;

			//描述触摸的状态。
			touchPhase::TouchPhase phase;

		};

	}
}


#endif // !CFLTOUCH_H
