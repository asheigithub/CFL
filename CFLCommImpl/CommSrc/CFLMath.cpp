#include "CFLMath.h"
#include <cmath>
#include <limits>


// Min and Max from WinDef.h
#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */




namespace cfl
{
	namespace math
	{
		 float sqrtf(float v){ return ::sqrtf(v); }

		 float abs(float v){ return fabs(v); }

		 bool unequalf(float v1, float v2)
		{
			const float EPSILON = std::numeric_limits<float>::epsilon();
			return ((v1< v2 -EPSILON ) || (v1 > (v2 + EPSILON)));

		}
		 bool equalf(float v1, float v2)
		 {
			 return !unequalf(v1, v2);
		 }

		 float cosf(float v){ return ::cosf(v); }
		 float sinf(float v){ return ::sinf(v); }

		 float tanf(float v){ return ::tanf(v); }

		 float minf(float v1, float v2)
		 {
			 return min(v1, v2);
		 }

		 float maxf(float v1, float v2)
		 {
			 return max(v1, v2);
		 }


	}
}


