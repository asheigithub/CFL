#ifndef CFL_COLOR_H
#define CFL_COLOR_H

#include "../CFLMath.h"

namespace cfl
{
	namespace graphic
	{

		/*
		RGBA�������ɫ��
		����ṹ������������ɫ��ÿ����ɫ�������һ����Χ��0��1֮��ĸ�����ֵ
		*/
		class Color
		{
		public:
			Color() :Color(0.0f, 0.0f, 0.0f, 1.0f){}

			Color(float r, float g, float b, float a)
				:r(math::clampf(r,0,1 ) ),
				g(math::clampf(g, 0, 1)),
				b(math::clampf(b, 0, 1)),
				a(math::clampf(a, 0, 1))
			{};
			~Color()=default;

		
			float r;
			float g;
			float b;
			float a;

			
		};

		

	}
}


#endif // !CFL_COLOR_H
