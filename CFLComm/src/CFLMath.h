#ifndef CFL_MATH_H
#define CFL_MATH_H




//--------------------------------------------------------------------------------------
// Degree/radian conversions
//--------------------------------------------------------------------------------------
const float PI = 3.1415926f;
//角度转弧度
#define toRadians( a )   ( (a) * (PI/180.0f) )
//弧度转角度
#define toDegrees( a )   ( (a) * (180.0f/PI) )



namespace cfl
{
	namespace math
	{
		 float sqrtf(float v);

		 float abs(float v);

		 bool equalf(float v1, float v2);

		 bool unequalf(float v1, float v2);

		 float cosf(float v);

		 float sinf(float v);

		 float tanf(float v);

		 float minf(float v1, float v2);
		 
		 float maxf(float v1, float v2);
		 
		 inline float clampf(float v, float min, float max)
		 {
			 if (v < min) return min;
			 if (v > max) return max;

			 return v;
		 }

	}
}







#endif // !CFL_MATH_H
