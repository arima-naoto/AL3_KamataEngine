#include "Calculation.h"

Vector3 Calculation::Add(const Vector3& v1, const Vector3& v2) 
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); 
}

Vector3 Calculation::Subtract(const Vector3& v1, const Vector3& v2) 
{
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 Calculation::MultiplyVector(float scalar, const Vector3& v) 
{
	return Vector3(scalar * v.x, scalar * v.y, scalar * v.z);
}

Matrix4x4 Calculation::MultiplyMatrix(const Matrix4x4& m1, const Matrix4x4& m2) 
{
	Matrix4x4 multiply{};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			multiply.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				multiply.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return multiply;
}