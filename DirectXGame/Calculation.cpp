#include "Calculation.h"
#define _USE_MATH_DEFINES
#include "cmath"

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

Vector3 Calculation::Normalize(const Vector3& v) {
	// 正規化を使用して計算結果を求める
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	Vector3 resultNormalize = {v.x, v.y, v.z};

	if (length != 0.0f) {
		resultNormalize = {v.x / length, v.y / length, v.z / length};
	}
	return resultNormalize;
}

bool Calculation::IsCollision(const AABB& aabb1, const AABB& aabb2) {

	//もし衝突していれば
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		return true;//戻り値をtrueに設定する
	}

	return false;


}