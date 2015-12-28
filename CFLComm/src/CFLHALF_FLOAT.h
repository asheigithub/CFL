#ifndef CFL_HALFFLOAT_H
#define CFL_HALFFLOAT_H



namespace cfl
{
	//typedef unsigned short FLOAT16;

	/*+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+*/

	extern unsigned short FloatToFloat16(float value);
	extern float Float16ToFloat(unsigned short value);

	/*+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+*/


	struct FLOAT16
	{
		FLOAT16() :mValue(0){}
		
		FLOAT16(float value);
		FLOAT16(const FLOAT16& value);

		


		unsigned short mValue;
		
	};
	
	inline FLOAT16::FLOAT16(float value)
	{
		mValue = FloatToFloat16(value);
	}

	/*+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+*/

	inline FLOAT16::FLOAT16(const FLOAT16 &value)
	{
		mValue = value.mValue;
	}

	



	
}







#endif // !CFL_HALFFLOAT_H
