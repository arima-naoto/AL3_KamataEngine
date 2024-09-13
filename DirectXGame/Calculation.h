#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

struct AABB {
	Vector3 min;
	Vector3 max;
};

class Calculation {

public:

	static Vector3 Add(const Vector3& v1, const Vector3& v2);

	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	static Vector3 MultiplyVector(float scalar, const Vector3& v);

	static Vector3 Multiply(const Vector3& vector, const Matrix4x4& matrix);

	static Matrix4x4 MultiplyMatrix(const Matrix4x4 &m1,const Matrix4x4 &m2);

	static Vector3 Normalize(const Vector3& v);

	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);

};