#ifndef WIN32INPUT_H
#define WIN32INPUT_H

#include "Input/CFLInput.h"

namespace cfl
{
	namespace input
	{
		struct InputState
		{
			InputState()
			{
				mousebuttonStates[0] = false;
				mousebuttonStates[1] = false;
				mousebuttonStates[2] = false;

				onMouseButtonDown[0] = false;
				onMouseButtonDown[1] = false;
				onMouseButtonDown[2] = false;

				onMouseButtonUp[0] = false;
				onMouseButtonUp[1] = false;
				onMouseButtonUp[2] = false;

				
			}

			bool mousebuttonStates[3] ;
			bool onMouseButtonDown[3] ;
			bool onMouseButtonUp[3] ;

			std::vector<Touch> touches;

			static InputState* getInstance()
			{
				static InputState instance;
				return &instance;
			}


			Touch currentToTouch;


			//ÿ֡ˢ��״̬
			void update(float dt)
			{
				onMouseButtonDown[0] = false;
				onMouseButtonDown[1] = false;
				onMouseButtonDown[2] = false;

				onMouseButtonUp[0] = false;
				onMouseButtonUp[1] = false;
				onMouseButtonUp[2] = false;

				static bool toclear = false;
				if (toclear)
				{
					
					touches.clear();
					toclear = false;

				}

				if (currentToTouch.phase == cfl::input::touchPhase::Began)
				{
					touches.clear();
					currentToTouch.deltaTime = 0;
					currentToTouch.deltaPosition = geom::Vector2();
					currentToTouch.fingerId = 0;

					touches.push_back(currentToTouch);

					currentToTouch.phase = cfl::input::touchPhase::Stationary;

				}
				else if (currentToTouch.phase == cfl::input::touchPhase::Ended)
				{
					if (touches.size() > 0)
					{
						auto newposition = input::Input::mousePositon();
						currentToTouch.deltaPosition = newposition - currentToTouch.position;
						currentToTouch.position = newposition;

						touches[0] = currentToTouch;
						toclear = true;
					}
				}
				else
				{
					if (touches.size() > 0)
					{
						
						auto newposition = input::Input::mousePositon();
						bool moved = !(newposition.equals(currentToTouch.position));
						
						if (moved)
						{
							currentToTouch.phase = cfl::input::touchPhase::Moved;
							currentToTouch.deltaTime = dt;
							
							currentToTouch.deltaPosition = newposition - currentToTouch.position;
							currentToTouch.position = newposition;

						}
						else
						{
							currentToTouch.phase = cfl::input::touchPhase::Stationary;
							currentToTouch.deltaTime =0;
							currentToTouch.deltaPosition = geom::Vector2();
						}
						
						*touches.begin() = currentToTouch;

					}

				}

			}


		};

	}
}


#endif // !WIN32INPUT_H
