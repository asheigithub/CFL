#include "CFLHALF_FLOAT.h"
#include <memory>

namespace cfl
{
	/*+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+*/

	unsigned short FloatToFloat16(float v)
	{

		unsigned int src = *(unsigned long*)(&v);
		unsigned int sign = src >> 31;

		// Extract mantissa
		unsigned int mantissa = src  &  (1 << 23) - 1;

		// extract & convert exp bits
		long exp = (long)(src >> 23 & 0xFF) - 127;
		if (exp > 16)
		{
			// largest possible number...
			exp = 16;
			mantissa = (1 << 23) - 1;
		}
		else if (exp <= -15)
		{
			// handle wraparound of shifts (done mod 64 on PPC)
			// All float32 denorm/0 values map to float16 0
			if (exp <= -14 - 24)
				mantissa = 0;
			else
				mantissa = (mantissa | 1 << 23) >> (-exp - 14);
			exp = -15;
		}

		return (unsigned short)(sign << 15) | (unsigned short)((exp + 15) << 10) | (unsigned short)(mantissa >> 13);
	}

	float Float16ToFloat(unsigned short fltInt16)
	{
		int fltInt32 = ((fltInt16 & 0x8000) << 16);
		fltInt32 |= ((fltInt16 & 0x7fff) << 13) + 0x38000000;

		float fRet;
		memcpy(&fRet, &fltInt32, sizeof(float));
		return fRet;
	}

}