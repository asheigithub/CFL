#include "clearwindows.h"
#include "Input/CFLInput.h"

#include "CFLEntry.h"
#include "AndroidInput.h"

namespace cfl
{
	namespace input
	{
		using namespace cfl::geom;

		//转换为游戏坐标
		Vector2 toGamePosition(const Vector2& input)
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
				float scale = stage->stageWidth() / viewport.width;

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
			return toGamePosition(std::move(Vector2(input::InputState::getInstance()->cursorx, input::InputState::getInstance()->cursory)));
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