#include "Geom/CFLGeom.h"
#include "CFLMath.h"

namespace cfl
{
	namespace geom
	{
		using namespace cfl::math;

		float Vector2::dot(const Vector2& vVector1) const
		{
			
			return (x * vVector1.x +
				y * vVector1.y);
		}

		float Vector2::getLength() const
		{
			return sqrtf(x * x + y * y);
		}

		Vector2  Vector2::getNormalize() const
		{
			float fLength = getLength();
			if (fLength != 0.0f)
				fLength = 1.0f / fLength;

			return Vector2(x * fLength, y * fLength);
		}

		Vector2&  Vector2::normalize()
		{
			float fLength = getLength();
			if (fLength != 0.0f)
				fLength = 1.0f / fLength;

			*this *= fLength;
			return *this;
		}



		Vector3 Vector3::mul(const Vector3& vVector, float fScalar)
		{
			Vector3 vResult;
			vResult.x = vVector.x * fScalar;
			vResult.y = vVector.y * fScalar;
			vResult.z = vVector.z * fScalar;
			return vResult;
		}

		Vector3 Vector3::add(const Vector3& vVector0, const Vector3& vVector1)
		{
			Vector3 vResult;
			vResult.x = vVector0.x + vVector1.x;
			vResult.y = vVector0.y + vVector1.y;
			vResult.z = vVector0.z + vVector1.z;
			return vResult;
		}

		Vector3 Vector3::cross(const Vector3& vVector0, const Vector3& vVector1)
		{
			Vector3 vResult;
			vResult.x = vVector0.y * vVector1.z - vVector0.z * vVector1.y;
			vResult.y = vVector0.z * vVector1.x - vVector0.x * vVector1.z;
			vResult.z = vVector0.x * vVector1.y - vVector0.y * vVector1.x;
			return vResult;
		}

		float Vector3::dot(const Vector3& vVector0, const Vector3& vVector1)
		{
			return (vVector0.x * vVector1.x +
				vVector0.y * vVector1.y +
				vVector0.z * vVector1.z);
		}

		float Vector3::getLength() const
		{
			return sqrtf(x * x +
				y * y +
				z * z);
		}

		Vector3 Vector3::getNormalize() const
		{
			float fLength = getLength();
			if (fLength != 0.0f)
				fLength = 1.0f / fLength;

			return Vector3(x,y,z) * fLength;
		}

		Vector3 Vector3::cross(const Vector3& vVector1) const
		{
			return Vector3::cross(*this, vVector1);
		}

		float Vector3::dot(const Vector3& vVector1) const
		{
			return Vector3::dot(*this, vVector1);
		}

		Vector3& Vector3::normalize()
		{
			float fLength = getLength();
			if (fLength != 0.0f)
				fLength = 1.0f / fLength;

			(*this) *= fLength;
			return *this;
		}






		float Vector4::dot(const Vector4& vVector0, const Vector4& vVector1)
		{
			return (vVector0.x * vVector1.x +
				vVector0.y * vVector1.y +
				vVector0.z * vVector1.z +
				vVector0.w * vVector1.w);
		}

		float   Vector4::getLength() const
		{
			return (x * x +
				y * y +
				z * z +
				w * w);
		}

		Vector4 Vector4::getNormalize() const
		{
			float fLength = getLength();
			if (fLength != 0.0f)
				fLength = 1.0f / fLength;

			return Vector4(x, y, z, w) * fLength;
		}

		float Vector4::dot(const Vector4& vVector1) const
		{
			return dot(*this, vVector1);
		}
		
		Vector4& Vector4::normalize()
		{
			float fLength = getLength();
			if (fLength != 0.0f)
				fLength = 1.0f / fLength;

			(*this) *= fLength;
			return *this;
		}


	}
}
