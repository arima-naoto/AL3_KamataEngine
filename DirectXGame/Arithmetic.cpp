#include "Arithmetic.h"
#include "Calculation.h"

Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Calculation::Add(v1, v2); }

Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Calculation::Subtract(v1, v2); }

Vector3 operator*(float s, const Vector3& v) { return Calculation::MultiplyVector(s, v); }

Vector3 operator*(const Vector3& v, float s) { return s * v; }

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return Calculation::MultiplyMatrix(m1, m2); }

Matrix4x4 operator*=(Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result; // 結果をm1に代入
	return m1;
}

Vector3 operator*=(Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}