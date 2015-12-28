#include "Geom/CFLGeom.h"

namespace cfl
{
	namespace geom
	{

		void Quaternion::setMatrix3D(Matrix3D* input)
		{

			//xx      = X * X;
			//xy      = X * Y;
			//xz      = X * Z;
			//xw      = X * W;
			//yy      = Y * Y;
			//yz      = Y * Z;
			//yw      = Y * W;
			//zz      = Z * Z;
			//zw      = Z * W;
			//mat[0]  = 1 - 2 * ( yy + zz );
			//mat[1]  =     2 * ( xy - zw );
			//mat[2]  =     2 * ( xz + yw );
			//mat[4]  =     2 * ( xy + zw );
			//mat[5]  = 1 - 2 * ( xx + zz );
			//mat[6]  =     2 * ( yz - xw );
			//mat[8]  =     2 * ( xz - yw );
			//mat[9]  =     2 * ( yz + xw );
			//mat[10] = 1 - 2 * ( xx + yy );
			//mat[3]  = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0;
			//mat[15] = 1;
			//
			//The resulting matrix uses the following positions:
			//
			//		mat[0]  mat[4] mat[ 8] mat[12] 
			//M =	mat[1]  mat[5] mat[ 9] mat[13] 
			//		mat[2]  mat[6] mat[10] mat[14] 
			//		mat[3]  mat[7] mat[11] mat[15] 

			float xx = qx * qx;
			float xy = qx * qy;
			float xz = qx * qz;
			float xw = qx * qw;
			float yy = qy * qy;
			float yz = qy * qz;
			float yw = qy * qw;
			float zz = qz * qz;
			float zw = qz * qw;

			//float* mat = input->rawData;
			input->M00 = 1 - 2 * (yy + zz);
			input->M10 = 2 * (xy - zw);
			input->M20 = 2 * (xz + yw);
			input->M01 = 2 * (xy + zw);
			input->M11 = 1 - 2 * (xx + zz);
			input->M21 = 2 * (yz - xw);
			input->M02 = 2 * (xz - yw);
			input->M12 = 2 * (yz + xw);
			input->M22 = 1 - 2 * (xx + yy);
			input->M30 = input->M31 = input->M32 = input->M03 = input->M13 = input->M23 = 0;
			input->M33 = 1;
		}

		Quaternion& Quaternion::fromMatrix3D(const Matrix3D& matrix)
		{
			float s;
			float tq[4];
			int    i, j;

			// Use tq to store the largest trace
			tq[0] = 1 + matrix.M[0][0] + matrix.M[1][1] + matrix.M[2][2];
			tq[1] = 1 + matrix.M[0][0] - matrix.M[1][1] - matrix.M[2][2];
			tq[2] = 1 - matrix.M[0][0] + matrix.M[1][1] - matrix.M[2][2];
			tq[3] = 1 - matrix.M[0][0] - matrix.M[1][1] + matrix.M[2][2];

			// Find the maximum (could also use stacked if's later)
			j = 0;
			for (i = 1; i<4; i++) j = (tq[i]>tq[j]) ? i : j;

			// check the diagonal
			if (j == 0)
			{

				qw = tq[0];
				qx = matrix.M[1][2] - matrix.M[2][1];
				qy = matrix.M[2][0] - matrix.M[0][2];
				qz = matrix.M[0][1] - matrix.M[1][0];
			}
			else if (j == 1)
			{
				qw = matrix.M[1][2] - matrix.M[2][1];
				qx = tq[1];
				qy = matrix.M[0][1] + matrix.M[1][0];
				qz = matrix.M[2][0] + matrix.M[0][2];
			}
			else if (j == 2)
			{
				qw = matrix.M[2][0] - matrix.M[0][2];
				qx = matrix.M[0][1] + matrix.M[1][0];
				qy = tq[2];
				qz = matrix.M[1][2] + matrix.M[2][1];
			}
			else
			{
				qw = matrix.M[0][1] - matrix.M[1][0];
				qx = matrix.M[2][0] + matrix.M[0][2];
				qy = matrix.M[1][2] + matrix.M[2][1];
				qz = tq[3];
			}

			s = math::sqrtf(0.25f / tq[j]);
			qw *= s;
			qx *= s;
			qy *= s;
			qz *= s;

			return *this;
		}
	}
}