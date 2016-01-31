#ifndef ANDROIDINPUT_H
#define ANDROIDINPUT_H

#include "Input/CFLInput.h"
#include "Geom/CFLGeom.h"
#include <vector>
#include <unordered_map>

namespace cfl
{
	namespace input
	{
		cfl::geom::Vector2 toGamePosition(const cfl::geom::Vector2& input);

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


			std::vector<Touch> touches;

			//缓存给更新用
			std::vector<Touch> touchTemp;
			//缓存上次的位置
			std::unordered_map<int, geom::Vector2> touchPos;

			//每帧刷新状态
			void update(float dt)
			{
				onMouseButtonDown[0] = false;
				onMouseButtonDown[1] = false;
				onMouseButtonDown[2] = false;

				onMouseButtonUp[0] = false;
				onMouseButtonUp[1] = false;
				onMouseButtonUp[2] = false;


				touches.clear();

				

				std::vector<int> toremovefingle;
				
				for (int i = 0; i != touchTemp.size(); i++)
				{
					Touch tempt = touchTemp[i];
					if (tempt.phase == input::touchPhase::Began)
					{
						touches.push_back(tempt);
						(&touchTemp[i])->phase = input::touchPhase::Stationary;

						auto find = touchPos.find(tempt.fingerId);
						if (find != touchPos.end())
						{
							find->second = tempt.position;
						}
						else
						{
							touchPos.emplace(std::make_pair(tempt.fingerId, tempt.position));
						}
						
					}
					else if (tempt.phase == input::touchPhase::Ended)
					{
						touches.push_back(tempt);
						auto find = touchPos.find(tempt.fingerId);
						if (find != touchPos.end())
						{
							touchPos.erase(find);
						}
						
						toremovefingle.push_back(tempt.fingerId);
						break;
					}
					else
					{
						
						auto find = touchPos.find(tempt.fingerId);
						if (find != touchPos.end())
						{
							auto oldpos = find->second;

							auto newposition = tempt.position;
							bool moved = !(newposition.equals(oldpos));

							find->second = newposition;

							if (moved)
							{
								tempt.phase = cfl::input::touchPhase::Moved;
								tempt.deltaTime = dt;

								tempt.deltaPosition = newposition - oldpos;
								tempt.position = newposition;

							}
							else
							{
								tempt.phase = cfl::input::touchPhase::Stationary;
								tempt.deltaTime = 0;
								tempt.deltaPosition = geom::Vector2();
							}
							touches.push_back(tempt);
						}
						else
						{
							//trace_e("Touch Id Not found");
							toremovefingle.push_back(tempt.fingerId);
							break;

						}
					}

				}

				for (size_t i = 0; i < toremovefingle.size(); i++)
				{
					int fid = toremovefingle[i];

					for (auto j = touchTemp.begin(); j !=touchTemp.end(); j++)
					{
						if (j->fingerId == fid)
						{
							touchTemp.erase(j);
							break;
						}
					}

				}
				
				
				
			}
		};

	}
}


#endif // !ANDROIDINPUT_H
