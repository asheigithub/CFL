#include "Input/CFLInput.h"

#include "Win32Input.h"

#include "CFLEntry.h"

namespace cfl
{
	namespace input
	{
		using namespace cfl::geom;

		//转换为游戏坐标
		static Vector2 toGamePosition(const Vector2& input)
		{
			auto stage = CFLContext::getCurrentInstance()->stage;

			auto scalemode = stage->scaleMode();

			switch (scalemode)
			{
			case cfl::EXACT_FIT:
				return Vector2(input);
				break;
			case cfl::SHOW_ALL:
				{
					auto viewport = stage->getGLViewPort();
					float scale = stage->stageWidth()/viewport.width  ;

					Vector2 ret = input;
					ret.x -= viewport.x;
					ret.y -= viewport.y;

					ret.x *= scale;
					ret.y *= scale;

					return ret;
				}
				break;
			case cfl::NO_BORDER:
				{
					auto viewport = stage->getGLViewPort();
					float scale = stage->stageWidth() / viewport.width;

					Vector2 ret = input;
					ret.x -= viewport.x;
					ret.y -= viewport.y;

					ret.x *= scale;
					ret.y *= scale;

					return ret;
				}
				break;
			case cfl::NO_SCALE:
				{
					auto viewport = stage->getGLViewPort();
					
					Vector2 ret = input;
					ret.x -= viewport.x;
					ret.y -= viewport.y;

					return ret;
				}
				break;
			default:
				return Vector2(input);
				break;
			}


		}


		Vector2 Input::mousePositon()
		{
			POINT      point;
			GetCursorPos(&point);

			ScreenToClient(CFLContext::getCurrentInstance()->eglNativeWindow,&point);

			return toGamePosition( std::move( Vector2(point.x, point.y)));

		}

		bool Input::getMouseButton(unsigned int button)
		{
			if (button > 2)
			{
				button = 2;
			}
			
			return InputState::getInstance()->mousebuttonStates[button];
		}

		bool Input::getMouseButtonDown(unsigned int button)
		{
			if (button > 2)
			{
				button = 2;
			}
			return InputState::getInstance()->onMouseButtonDown[button];
		}

		bool Input::getMouseButtonUp(unsigned int button)
		{
			if (button > 2)
			{
				button = 2;
			}
			return InputState::getInstance()->onMouseButtonUp[button];
		}


		std::vector<Touch>* Input::getTouches()
		{
			return &(InputState::getInstance()->touches);
		}

	}
}