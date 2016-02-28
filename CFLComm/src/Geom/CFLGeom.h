#ifndef CFL_GEOM_H
#define CFL_GEOM_H

#include "../CFLMath.h"
#include <memory>
namespace cfl
{
	namespace geom
	{
		struct Matrix;

		//2个成员的向量 x,y
		struct Vector2
		{

			Vector2() :x(0), y(0) {}
			Vector2(float _x, float _y) { x = _x; y = _y; }
			Vector2& operator  = (const Vector2& val) { x = val.x; y = val.y; return *this; }
			Vector2& operator += (const Vector2& val) { x += val.x; y += val.y; return *this; }
			Vector2& operator -= (const Vector2& val) { x -= val.x; y -= val.y; return *this; }
			Vector2& operator *= (float val) { x *= val; y *= val; return *this; }
			Vector2& operator /= (float val) { x /= val; y /= val; return *this; }
			Vector2  operator + () const { return Vector2(x, y); }
			Vector2  operator - () const { return Vector2(-x, -y); }
			Vector2  operator + (const Vector2& val) const { return Vector2(x + val.x, y + val.y); }
			Vector2  operator - (const Vector2& val) const { return Vector2(x - val.x, y - val.y); }
			Vector2  operator * (float val) const { return Vector2(x*val, y*val); }
			Vector2  operator / (float val) const { return Vector2(x / val, y / val); }
			friend Vector2 operator * (float s, const Vector2& v){ return Vector2(s*v.x, s*v.y); }

			//求2个2维向量的点积
			float    dot(const Vector2& vVector1) const;
			float    getLength() const;
			Vector2  getNormalize() const;

			Vector2&  normalize();

			bool equals(const Vector2& v2) const
			{
				return math::equalf(x, v2.x) && math::equalf(y, v2.y);
			}

			bool operator==(const Vector2& v2) const
			{
				return equals(v2);
			}
			bool operator !=(const Vector2& v2) const
			{
				return !equals(v2);
			}

			inline Vector2 operator*(const Matrix& matrix) const;
			
			union
			{
				struct{ float x, y; };
				struct{ float tu, tv; };
				float v[2];
			};
		};
		struct Matrix3D;
		//三个成员的向量 x,y,z
		struct Vector3
		{

			Vector3() :x(0), y(0), z(0) {}
			Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
			Vector3& operator  = (const Vector3& val) { x = val.x; y = val.y; z = val.z; return *this; }
			Vector3& operator += (const Vector3& val) { x += val.x; y += val.y; z += val.z; return *this; }
			Vector3& operator -= (const Vector3& val) { x -= val.x; y -= val.y; z -= val.z; return *this; }
			Vector3& operator *= (float val) { x *= val; y *= val; z *= val; return *this; }
			Vector3& operator /= (float val) { x /= val; y /= val; z /= val; return *this; }
			Vector3  operator + () const { return Vector3(x, y, z); }
			Vector3  operator - () const { return Vector3(-x, -y, -z); }
			Vector3  operator + (const Vector3& val) const { return Vector3(x + val.x, y + val.y, z + val.z); }
			Vector3  operator - (const Vector3& val) const { return Vector3(x - val.x, y - val.y, z - val.z); }
			Vector3  operator * (float val) const { return Vector3(x*val, y*val, z*val); }
			Vector3  operator / (float val) const { return Vector3(x / val, y / val, z / val); }
			friend Vector3 operator * (float s, const Vector3& v){ return Vector3(s*v.x, s*v.y, s*v.z); }


			union
			{
				struct{ float x, y, z; };
				struct{ float r, g, b; };
				float v[3];
			};




			static Vector3 mul(const Vector3& vVector,float fScalar);
			static Vector3 add(const Vector3& vVector0,const Vector3& vVector1);
			static Vector3 cross(const Vector3& vVector0,const Vector3& vVector1);
			static float    dot(const Vector3& vVector0,const Vector3& vVector1);
			
			float   getLength() const;
			Vector3 getNormalize() const;

			Vector3 cross(const Vector3& vVector1) const;
			float dot(const Vector3& vVector1) const;
			Vector3& normalize();

			inline Vector3 operator*(const Matrix3D& matrix) const;
		};



		//四个成员的向量 x,y,z,w
		struct Vector4
		{
			

			union
			{
				struct{ float x, y, z, w; };
				struct{ float r, g, b, a; };
				float v[4];
			};


			Vector4():x(0),y(0),z(0),w(1) {}
			Vector4(float _x, float _y, float _z) :Vector4(_x, _y, _z, 1){}
			Vector4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
			Vector4(Vector3 v) :Vector4(v, 1){}
			Vector4(Vector3 v, float _w) { x = v.x; y = v.y; z = v.z; w = _w; }
			Vector4& operator  = (const Vector4& val) { x = val.x; y = val.y; z = val.z; w = val.w; return *this; }
			Vector4& operator += (const Vector4& val) { x += val.x; y += val.y; z += val.z; w += val.w; return *this; }
			Vector4& operator -= (const Vector4& val) { x -= val.x; y -= val.y; z -= val.z; w -= val.w; return *this; }
			Vector4& operator *= (float val) { x *= val; y *= val; z *= val; w *= val; return *this; }
			Vector4& operator /= (float val) { x /= val; y /= val; z /= val; w /= val; return *this; }
			Vector4  operator + () const { return Vector4(x, y, z, w); }
			Vector4  operator - () const { return Vector4(-x, -y, -z, -w); }
			Vector4  operator + (const Vector4& val) const { return Vector4(x + val.x, y + val.y, z + val.z, w + val.w); }
			Vector4  operator - (const Vector4& val) const { return Vector4(x - val.x, y - val.y, z - val.z, w - val.w); }
			Vector4  operator * (float val) const { return Vector4(x*val, y*val, z*val, w*val); }
			Vector4  operator / (float val) const { return Vector4(x / val, y / val, z / val, w / val); }
			friend Vector4 operator * (float s, const Vector4& v){ return Vector4(s*v.x, s*v.y, s*v.z, s*v.w); }



			static float    dot(const Vector4& vVector0, const Vector4& vVector1);
			float   getLength() const;
			Vector4 getNormalize() const;

			float dot(const Vector4& vVector1) const;
			Vector4& normalize();

			inline Vector4 operator*(const Matrix3D& matrix) const;

		};
	


		const Vector4 X_AXIS = Vector4(1, 0, 0, 0);
		const Vector4 Y_AXIS = Vector4(0, 1, 0, 0);
		const Vector4 Z_AXIS = Vector4(0, 0, 1, 0);

		
		//四元数
		struct Quaternion
		{
			Quaternion() :qx(0), qy(0), qz(0), qw(1){}
			//从旋转轴和旋转角度设置四元数值
			Quaternion& fromAxisAngle(const Vector3& axis,float angle )
			{
				
				float sin_a = math::sinf(angle / 2);
				float cos_a = math::cosf(angle / 2);

				float len = 1/axis.getLength();

				qx = axis.x * len * sin_a;
				qy = axis.y * len * sin_a;
				qz = axis.z * len * sin_a;
				qw = cos_a;

				return *this;
			}

			//从旋转矩阵设置四元数
			Quaternion& fromMatrix3D(const Matrix3D& matrix);



			//转化为一个旋转矩阵
			void setMatrix3D(Matrix3D* input);

			float qx;
			float qy;
			float qz;
			float qw;
		};
		

		//计算2阶方阵的行列式
		inline float DET2X2(float m00, float m01,
			float m10, float m11)
		{
			return m00 * m11 - m01 * m10;
		}

		//计算3阶方阵的行列式
		inline float DET3X3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22)
		{
			return m00 * DET2X2(m11, m12, m21, m22) -
				m10 * DET2X2(m01, m02, m21, m22) +
				m20 * DET2X2(m01, m02, m11, m12);
		}

		//计算4阶方阵的行列式
		inline float DET4X4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			return m00 * DET3X3(m11, m12, m13, m21, m22, m23, m31, m32, m33) -
				m10 * DET3X3(m01, m02, m03, m21, m22, m23, m31, m32, m33) +
				m20 * DET3X3(m01, m02, m03, m11, m12, m13, m31, m32, m33) -
				m30 * DET3X3(m01, m02, m03, m11, m12, m13, m21, m22, m23);
		}



		/*2D矩阵类。采用列向量存储		
			a 	b 	0
			c 	d 	0
			tx	ty	1
		*/
		struct Matrix
		{
			union
			{
				struct 
				{ 
					float a, b, 
						//永远为0。。设置了也算0
						u, 
						c, d, 
						//永远为0。。设置了也算0
						v, 
						tx, ty, 
						//永远为1。。设置了也算1
						w;
				};
				float M[3][3];
				float mat[9];
			};

			Matrix():Matrix(1,0,0,1,0,0){}
			Matrix(float a, float b, float c, float d, float tx, float ty) :
				a(a), b(b), c(c), d(d), tx(tx), ty(ty), u(0), v(0), w(1){}
			
			//将本矩阵初始化为单位阵
			Matrix& identity()
			{
				a = 1; b = 0; c = 0; d = 1; tx = 0; ty = 0;
				return *this;
			}

			//将当前矩阵转化为当前矩阵的逆矩阵
			Matrix& invert()
			{
				/*If Kramer's rule is applied to a matrix M:
					| A B C |
				M = | D E F |
					| G H I |
				then the determinant is calculated as follows:
				det M = A * (EI - HF) - B * (DI - GF) + C * (DH - GE)

				Providing that the determinant is non-zero, then the inverse is
				calculated as:
			     -1     1     |   EI-FH  -(BI-HC)   BF-EC  |
				M   = ----- . | -(DI-FG)   AI-GC  -(AF-DC) |
					  det M   |   DH-GE  -(AH-GB)   AE-BD  |

				*/

				float det;
				det = mat[0] * (mat[4])// * mat[8])// - mat[7] * mat[5])
					- mat[1] * (mat[3]);// *mat[8]);// - mat[6] * mat[5]); mat[5]必==0
					//+ mat[2] * (mat[3] * mat[7] - mat[6] * mat[4]); C必==0
		
				if (math::equalf(det, 0))
				{
					return identity();
				}

				Matrix temp = *this;
				auto ma = temp.mat;

				/*
				A0	B1	C2
				D3	E4	F5
				G6	H7	I8
				*/

				det = 1 / det;

				mat[0] = (ma[4] * ma[8] - ma[5] * ma[7]) * det;   //EI - FH
				mat[1] = -(ma[1] * ma[8] - ma[7] * ma[2]) * det;  //-(BI-HC)
				mat[2] = 0;// (ma[1] * ma[5] - ma[4] * ma[2]) / det;   //BF-EC
				
				mat[3] = -(ma[3] * ma[8] - ma[5] * ma[6]) * det;  //-(DI-FG)   
				mat[4] =( ma[0] * ma[8] - ma[6] * ma[2]) * det;   //AI-GC 
				mat[5] = 0;// -(ma[0] * ma[5] - ma[2] * ma[3]) / det;  // -(AF-DC)
				
				mat[6] =( ma[3] * ma[7] - ma[6] * ma[4]) * det;	  //DH-GE  
				mat[7] = -(ma[0] * ma[7] - ma[1] * ma[6]) * det;  //-(AH-GB)   
				mat[8] = 1;// (ma[0] * ma[4] - ma[1] * ma[3]) / det;   //AE-BD


				return *this;

			}
			//求当前矩阵的逆矩阵
			Matrix getInvert() const
			{
				Matrix ret = *this;
				return ret.invert();
			}

			Matrix& operator *=(const Matrix& rhs)
			{
				return append(rhs);
			}

			Matrix& append(const Matrix& rhs)
			{
				/*
				a1 	b1 	u1		a2	b2	u2
				c1 	d1 	v1	*	c2	d2	v2
				tx1	ty1	w1		tx2	ty2	w2
				*/

				Matrix lhs= *this;
				a = lhs.a * rhs.a + lhs.b * rhs.c;// +lhs.u * rhs.tx;
				b = lhs.a * rhs.b + lhs.b * rhs.d;// +lhs.u * rhs.ty;
				u = 0;// lhs.a * rhs.u + lhs.c * rhs.v + lhs.u * rhs.w;

				c = lhs.c * rhs.a + lhs.d * rhs.c;// +lhs.v * rhs.tx;
				d = lhs.c * rhs.b + lhs.d * rhs.d;// +lhs.v * rhs.ty;
				v = 0;// lhs.b * rhs.u + lhs.d * rhs.v + lhs.v * rhs.w;

				tx = lhs.tx * rhs.a + lhs.ty * rhs.c + /*lhs.w * */ rhs.tx;
				ty = lhs.tx * rhs.b + lhs.ty * rhs.d + /*lhs.w * */ rhs.ty;
				w = 1;// lhs.tx * rhs.u + lhs.ty * rhs.v + lhs.w * rhs.w;


				return *this;
			}

			Matrix operator *(const Matrix& rhs) const
			{
				Matrix ret = *this;
				ret *= rhs;
				return ret;
			}

			
			//对当前矩阵应用一个旋转
			Matrix& rotate(float radians)
			{
				return append(
					Matrix(math::cosf(radians),  math::sinf(radians), 
						 -math::sinf(radians), math::cosf(radians), 0, 0));
			}

			Matrix& scale(float sx,float sy)
			{
				return append(
					Matrix(sx, 0, 0, sy, 0, 0)
					);
			}


			//对当前矩阵追加平移
			Matrix& translate(float dx,float dy)
			{
				//**w值永远为1,所以直接加上即可
				tx += dx;
				ty += dy;

				return *this;
			}

			Vector2 transformPoint(const Vector2& vector) const
			{
				/*			
								| a	 b  u|
					[x,y,1]	*	| c  d  v|
								| tx ty w|
				*/

				return Vector2(vector.x * a + vector.y * c + tx, vector.x * b + vector.y * d + ty);
			}

			


		};

		/*
		3D矩阵类 4x4,列向量存储
			| 0  4  8  12 |
		    |             |
		    | 1  5  9  13 |
			|             |
		    | 2  6  10 14 |
		    |             |
		    | 3  7  11 15 |
		*/
		struct Matrix3D
		{
			union
			{
				struct
				{
					float M00, M01, M02, M03,
						M10, M11, M12, M13,
						M20, M21, M22, M23,
						M30, M31, M32, M33
						;

				};

				float M[4][4];
				float rawData[16];
			};

			Matrix3D() :Matrix3D(Vector4(1, 0, 0, 0),
				Vector4(0, 1, 0, 0),
				Vector4(0, 0, 1, 0),
				Vector4(0, 0, 0, 1)){}

			Matrix3D(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
				: M00(m00), M01(m01), M02(m02), M03(m03),
				M10(m10), M11(m11), M12(m12), M13(m13),
				M20(m20), M21(m21), M22(m22), M23(m23),
				M30(m30), M31(m31), M32(m32), M33(m33)
			{}

			//按4列初始化矩阵
			Matrix3D(const Vector4& col1, const Vector4& col2, const Vector4& col3, const Vector4& col4)
				:M00(col1.x), M01(col2.x), M02(col3.x), M03(col4.x),
				M10(col1.y), M11(col2.y), M12(col3.y), M13(col4.y),
				M20(col1.z), M21(col2.z), M22(col3.z), M23(col4.z),
				M30(col1.w), M31(col2.w), M32(col3.w), M33(col4.w)
			{}
			//从一个2D变换矩阵转到3D变换矩阵，将tx,ty,移动到第四行
			Matrix3D(const Matrix mat) :Matrix3D(
				Vector4(mat.a, mat.c, 0, mat.tx),
				Vector4(mat.b, mat.d, 0, mat.ty),
				Vector4(0, 0, 1, 0),
				Vector4(0, 0, 0, 1)
				)
			{}

			Matrix3D& identity()
			{
				M00 = 1; M01 = 0; M02 = 0; M03 = 0;
				M10 = 0; M11 = 1; M12 = 0; M13 = 0;
				M20 = 0; M21 = 0; M22 = 1; M23 = 0;
				M30 = 0; M31 = 0; M32 = 0; M33 = 1;

				return *this;
			}


			//通过将另一个 Matrix3D 对象与当前 Matrix3D 对象相乘来后置一个矩阵。
			Matrix3D& append(const Matrix3D& rhs)
			{
				Matrix3D lhs = *this;

				auto pSrc1 = (&lhs)->M;
				auto pSrc2 = (&rhs)->M;


				M[0][0] = pSrc1[0][0] * pSrc2[0][0] + pSrc1[0][1] * pSrc2[1][0] + pSrc1[0][2] * pSrc2[2][0] + pSrc1[0][3] * pSrc2[3][0];
				M[0][1] = pSrc1[0][0] * pSrc2[0][1] + pSrc1[0][1] * pSrc2[1][1] + pSrc1[0][2] * pSrc2[2][1] + pSrc1[0][3] * pSrc2[3][1];
				M[0][2] = pSrc1[0][0] * pSrc2[0][2] + pSrc1[0][1] * pSrc2[1][2] + pSrc1[0][2] * pSrc2[2][2] + pSrc1[0][3] * pSrc2[3][2];
				M[0][3] = pSrc1[0][0] * pSrc2[0][3] + pSrc1[0][1] * pSrc2[1][3] + pSrc1[0][2] * pSrc2[2][3] + pSrc1[0][3] * pSrc2[3][3];

				M[1][0] = pSrc1[1][0] * pSrc2[0][0] + pSrc1[1][1] * pSrc2[1][0] + pSrc1[1][2] * pSrc2[2][0] + pSrc1[1][3] * pSrc2[3][0];
				M[1][1] = pSrc1[1][0] * pSrc2[0][1] + pSrc1[1][1] * pSrc2[1][1] + pSrc1[1][2] * pSrc2[2][1] + pSrc1[1][3] * pSrc2[3][1];
				M[1][2] = pSrc1[1][0] * pSrc2[0][2] + pSrc1[1][1] * pSrc2[1][2] + pSrc1[1][2] * pSrc2[2][2] + pSrc1[1][3] * pSrc2[3][2];
				M[1][3] = pSrc1[1][0] * pSrc2[0][3] + pSrc1[1][1] * pSrc2[1][3] + pSrc1[1][2] * pSrc2[2][3] + pSrc1[1][3] * pSrc2[3][3];

				M[2][0] = pSrc1[2][0] * pSrc2[0][0] + pSrc1[2][1] * pSrc2[1][0] + pSrc1[2][2] * pSrc2[2][0] + pSrc1[2][3] * pSrc2[3][0];
				M[2][1] = pSrc1[2][0] * pSrc2[0][1] + pSrc1[2][1] * pSrc2[1][1] + pSrc1[2][2] * pSrc2[2][1] + pSrc1[2][3] * pSrc2[3][1];
				M[2][2] = pSrc1[2][0] * pSrc2[0][2] + pSrc1[2][1] * pSrc2[1][2] + pSrc1[2][2] * pSrc2[2][2] + pSrc1[2][3] * pSrc2[3][2];
				M[2][3] = pSrc1[2][0] * pSrc2[0][3] + pSrc1[2][1] * pSrc2[1][3] + pSrc1[2][2] * pSrc2[2][3] + pSrc1[2][3] * pSrc2[3][3];

				M[3][0] = pSrc1[3][0] * pSrc2[0][0] + pSrc1[3][1] * pSrc2[1][0] + pSrc1[3][2] * pSrc2[2][0] + pSrc1[3][3] * pSrc2[3][0];
				M[3][1] = pSrc1[3][0] * pSrc2[0][1] + pSrc1[3][1] * pSrc2[1][1] + pSrc1[3][2] * pSrc2[2][1] + pSrc1[3][3] * pSrc2[3][1];
				M[3][2] = pSrc1[3][0] * pSrc2[0][2] + pSrc1[3][1] * pSrc2[1][2] + pSrc1[3][2] * pSrc2[2][2] + pSrc1[3][3] * pSrc2[3][2];
				M[3][3] = pSrc1[3][0] * pSrc2[0][3] + pSrc1[3][1] * pSrc2[1][3] + pSrc1[3][2] * pSrc2[2][3] + pSrc1[3][3] * pSrc2[3][3];

				return *this;
			}

			//转置当前矩阵。
			Matrix3D& transpose()
			{
				//std::swap(M[0][0], M[0][0]);
				std::swap(M[0][1], M[1][0]);
				std::swap(M[0][2], M[2][0]);
				std::swap(M[0][3], M[3][0]);

				//std::swap(M[1][0], M[0][1]);
				//std::swap(M[1][1], M[1][1]);
				std::swap(M[1][2], M[2][1]);
				std::swap(M[1][3], M[3][1]);

				//std::swap(M[2][0], M[0][2]);
				//std::swap(M[2][1], M[1][2]);
				//std::swap(M[2][2], M[2][2]);
				std::swap(M[2][3], M[3][2]);

				//std::swap(M[3][0], M[0][3]);
				//std::swap(M[3][1], M[1][3]);
				//std::swap(M[3][2], M[2][3]);
				//std::swap(M[3][3], M[3][3]);
				return *this;
			}
			//获取当前矩阵的转置矩阵
			Matrix3D getTranspose() const
			{
				Matrix3D ret = *this;
				return ret.transpose();
			}


			Matrix3D operator*(const Matrix3D& rhs) const
			{
				Matrix3D ret = *this;
				return ret.append(rhs);
			}

			//获取当前矩阵的行列式。如果值为0，则不可逆。Matrix3D必须是可逆的矩阵
			float determinant() const
			{
				float fDeterminant = DET4X4(M[0][0], M[0][1], M[0][2], M[0][3],
					M[1][0], M[1][1], M[1][2], M[1][3],
					M[2][0], M[2][1], M[2][2], M[2][3],
					M[3][0], M[3][1], M[3][2], M[3][3]);

				return fDeterminant;

			}

			//将当前矩阵转化为当前矩阵的逆矩阵
			Matrix3D& invert()
			{
				float fDeterminant = determinant();

				if (math::equalf(fDeterminant, 0))
				{
					this->identity();
					return *this;
				}

				float fScale = 1.0f / fDeterminant;

				Matrix3D s = *this;

				M[0][0] = +DET3X3(s.M[1][1], s.M[1][2], s.M[1][3], s.M[2][1], s.M[2][2], s.M[2][3], s.M[3][1], s.M[3][2], s.M[3][3]) * fScale;
				M[1][0] = -DET3X3(s.M[1][0], s.M[1][2], s.M[1][3], s.M[2][0], s.M[2][2], s.M[2][3], s.M[3][0], s.M[3][2], s.M[3][3]) * fScale;
				M[2][0] = +DET3X3(s.M[1][0], s.M[1][1], s.M[1][3], s.M[2][0], s.M[2][1], s.M[2][3], s.M[3][0], s.M[3][1], s.M[3][3]) * fScale;
				M[3][0] = -DET3X3(s.M[1][0], s.M[1][1], s.M[1][2], s.M[2][0], s.M[2][1], s.M[2][2], s.M[3][0], s.M[3][1], s.M[3][2]) * fScale;

				M[0][1] = -DET3X3(s.M[0][1], s.M[0][2], s.M[0][3], s.M[2][1], s.M[2][2], s.M[2][3], s.M[3][1], s.M[3][2], s.M[3][3]) * fScale;
				M[1][1] = +DET3X3(s.M[0][0], s.M[0][2], s.M[0][3], s.M[2][0], s.M[2][2], s.M[2][3], s.M[3][0], s.M[3][2], s.M[3][3]) * fScale;
				M[2][1] = -DET3X3(s.M[0][0], s.M[0][1], s.M[0][3], s.M[2][0], s.M[2][1], s.M[2][3], s.M[3][0], s.M[3][1], s.M[3][3]) * fScale;
				M[3][1] = +DET3X3(s.M[0][0], s.M[0][1], s.M[0][2], s.M[2][0], s.M[2][1], s.M[2][2], s.M[3][0], s.M[3][1], s.M[3][2]) * fScale;

				M[0][2] = +DET3X3(s.M[0][1], s.M[0][2], s.M[0][3], s.M[1][1], s.M[1][2], s.M[1][3], s.M[3][1], s.M[3][2], s.M[3][3]) * fScale;
				M[1][2] = -DET3X3(s.M[0][0], s.M[0][2], s.M[0][3], s.M[1][0], s.M[1][2], s.M[1][3], s.M[3][0], s.M[3][2], s.M[3][3]) * fScale;
				M[2][2] = +DET3X3(s.M[0][0], s.M[0][1], s.M[0][3], s.M[1][0], s.M[1][1], s.M[1][3], s.M[3][0], s.M[3][1], s.M[3][3]) * fScale;
				M[3][2] = -DET3X3(s.M[0][0], s.M[0][1], s.M[0][2], s.M[1][0], s.M[1][1], s.M[1][2], s.M[3][0], s.M[3][1], s.M[3][2]) * fScale;

				M[0][3] = -DET3X3(s.M[0][1], s.M[0][2], s.M[0][3], s.M[1][1], s.M[1][2], s.M[1][3], s.M[2][1], s.M[2][2], s.M[2][3]) * fScale;
				M[1][3] = +DET3X3(s.M[0][0], s.M[0][2], s.M[0][3], s.M[1][0], s.M[1][2], s.M[1][3], s.M[2][0], s.M[2][2], s.M[2][3]) * fScale;
				M[2][3] = -DET3X3(s.M[0][0], s.M[0][1], s.M[0][3], s.M[1][0], s.M[1][1], s.M[1][3], s.M[2][0], s.M[2][1], s.M[2][3]) * fScale;
				M[3][3] = +DET3X3(s.M[0][0], s.M[0][1], s.M[0][2], s.M[1][0], s.M[1][1], s.M[1][2], s.M[2][0], s.M[2][1], s.M[2][2]) * fScale;



				return *this;

			}
			//获取当前矩阵的逆矩阵
			Matrix3D getInvert() const
			{
				Matrix3D ret = *this;
				return ret.invert();
			}

			Matrix3D& appendRotation(float angle, Vector4 axis)
			{
				Quaternion q;
				q.fromAxisAngle(Vector3(axis.x, axis.y, axis.z), angle);

				Matrix3D rot;
				q.setMatrix3D(&rot);

				this->append(rot);
				return *this;
			}

			Matrix3D& appendScale(float xScale, float yScale, float zScale)
			{
				/*this->append(Matrix3D(
					xScale, 0.0f, 0.0f, 0.0f,
					0.0f, yScale, 0.0f, 0.0f,
					0.0f, 0.0f, zScale, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f));*/



				M[0][0] = M[0][0] *xScale + 0 + 0 + 0;
				M[0][1] = 0 + M[0][1] * yScale + 0 + 0;
				M[0][2] = 0 + 0 + M[0][2] * zScale + 0;
				M[0][3] = 0 + 0 + 0 + M[0][3];

				M[1][0] = M[1][0] * xScale + 0 + 0 + 0;
				M[1][1] = 0  + M[1][1] * yScale + 0 + 0;
				M[1][2] = 0 + 0 + M[1][2] * zScale + 0;
				M[1][3] = 0 + 0 + 0 + M[1][3] ;

				M[2][0] = M[2][0] * xScale + 0 + 0 + 0;
				M[2][1] = 0 + M[2][1] * yScale + 0 + 0;
				M[2][2] = 0 + 0 + M[2][2] * zScale + 0;
				M[2][3] = 0 + 0 + 0 + M[2][3];

				M[3][0] = M[3][0] * xScale + 0 + 0 + 0;
				M[3][1] = 0 + M[3][1] * yScale +0 + 0;
				M[3][2] = 0 + 0 + M[3][2] * zScale + 0;
				M[3][3] = 0 + 0 + 0 + M[3][3];



				return *this;

			}

			Matrix3D& appendTranslation(float x, float y, float z)
			{
				/*this->append(Matrix3D(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					x, y, z, 1.0f));*/


				M[0][0] = M[0][0]  + 0 + 0 + M[0][3]*x;
				M[0][1] = 0 + M[0][1] + 0 + M[0][3]*y;
				M[0][2] = 0 + 0 + M[0][2]  + M[0][3]*z;
				M[0][3] = 0 + 0 + 0 + M[0][3] ;

				M[1][0] = M[1][0]  + 0 + 0 + M[1][3] * x;
				M[1][1] =0 + M[1][1]  + 0 + M[1][3] * y;
				M[1][2] = 0 + 0 + M[1][2] + M[1][3] * z;
				M[1][3] = 0 + 0 + 0 + M[1][3] ;

				M[2][0] = M[2][0]  +0 + 0 + M[2][3] * x;
				M[2][1] = 0 + M[2][1]  + 0 + M[2][3] * y;
				M[2][2] = 0 + 0 + M[2][2]  + M[2][3] * z;
				M[2][3] = 0 + 0 + 0 + M[2][3] ;

				M[3][0] = M[3][0]  + 0 + 0 + M[3][3] * x;
				M[3][1] = 0 + M[3][1]  + 0+ M[3][3] * y;
				M[3][2] = 0 + 0 + M[3][2] + M[3][3] * z;
				M[3][3] = 0 + 0 + 0 + M[3][3];


				return *this;
			}






			static  Matrix3D lookAtLH(
				float eyex, float eyey, float eyez,
				float lookatx, float lookaty, float lookatz,
				float upx, float upy, float upz)
			{
				// 相机UVN
				//N= lookAt- eye
				//U=UP * N
				//V = N *U

				Vector3 lookat = Vector3(lookatx, lookaty, lookatz);
				Vector3 eye = Vector3(eyex, eyey, eyez);
				Vector3 up = Vector3(upx, upy, upz);


				auto  N = lookat - eye;

				auto U = up.cross(N); //Vector3.Cross(up, N);

				auto V = N.cross(U); //Vector3.Cross(N, U);

				N.normalize();
				U.normalize();
				V.normalize();


				Matrix3D m1 = Matrix3D(U.x, U.y, U.z, 0.0f,
					V.x, V.y, V.z, 0.0f,
					N.x, N.y, N.z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
				Matrix3D m2 = Matrix3D(

					1.0f, 0.0f, 0.0f, -eyex,
					0.0f, 1.0f, 0.0f, -eyey,
					0.0f, 0.0f, 1.0f, -eyez,
					0.0f, 0.0, 0.0, 1.0f

					);

				m1.append(m2);
				m1.transpose();
				return m1;
			}

			static Matrix3D perspectiveLH(float fov, float aspect, float znear, float zfar)
			{
				float top = znear *  math::tanf(fov / 2);
				float bottom = -top;
				float right = top * aspect;
				float left = -right;

				float a = zfar / (zfar - znear);
				float b = -znear * zfar / (zfar - znear);

				Matrix3D m(
					2.0f * znear / (right - left), 0.0f, (left + right) / (left - right), 0.0f,
					0.0f, 2.0f* znear / (top - bottom), (bottom + top) / (bottom - top), 0.0f,
					0.0f, 0.0f, a, b,
					0.0f, 0.0f, 1.0f, 0.0f
					);

				m.transpose();
				return m;


			}


			static Matrix3D perspectiveOffCenterLH(float left,
				float right,
				float bottom,
				float top,
				float zNear,
				float zFar)
			{

				float a = zFar / (zFar - zNear);
				float b = -zNear * zFar / (zFar - zNear);


				Matrix3D m(
					2.0f * zNear / (right - left), 0.0f, (left + right) / (left - right), 0.0f,
					0.0f, 2.0f * zNear / (top - bottom), (bottom + top) / (bottom - top), 0.0f,
					0.0f, 0.0f, a, b,
					0.0, 0.0, 1.0f, 0.0
					);
				m.transpose();
				return m;
			}

		

		};






		//***重载向量与矩阵乘法****
		inline Vector2 Vector2::operator*(const Matrix& matrix) const
		{
			return Vector2(x * matrix.a + y * matrix.c + matrix.tx, x * matrix.b + y * matrix.d + matrix.ty);
		}

		inline Vector3 Vector3::operator*(const Matrix3D& matrix) const
		{
			return Vector3(
				x * matrix.M00 + y * matrix.M10 + z * matrix.M20 + matrix.M30,
				x * matrix.M01 + y * matrix.M11 + z * matrix.M21 + matrix.M31,
				x * matrix.M02 + y * matrix.M12 + z * matrix.M22 + matrix.M32
				);
		}

		inline Vector4 Vector4::operator*(const Matrix3D& matrix) const
		{
			return Vector4(
				x * matrix.M00 + y * matrix.M10 + z * matrix.M20 + w* matrix.M30,
				x * matrix.M01 + y * matrix.M11 + z * matrix.M21 + w* matrix.M31,
				x * matrix.M02 + y * matrix.M12 + z * matrix.M22 + w* matrix.M32,
				x * matrix.M03 + y * matrix.M13 + z * matrix.M23 + w* matrix.M33
				);
		}

	}

}





#endif // !CFL_GEOM_H
