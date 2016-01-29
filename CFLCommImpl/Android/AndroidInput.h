#ifndef ANDROIDINPUT_H
#define ANDROIDINPUT_H

namespace cfl
{
	namespace input
	{
		struct InputState
		{
			float cursorx;
			float cursory;

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

			bool mousebuttonStates[3];
			bool onMouseButtonDown[3];
			bool onMouseButtonUp[3];

			static InputState* getInstance()
			{
				static InputState instance;
				return &instance;
			}


			//ÿ֡ˢ��״̬
			void update()
			{
				onMouseButtonDown[0] = false;
				onMouseButtonDown[1] = false;
				onMouseButtonDown[2] = false;

				onMouseButtonUp[0] = false;
				onMouseButtonUp[1] = false;
				onMouseButtonUp[2] = false;

			}
		};

	}
}


#endif // !ANDROIDINPUT_H
