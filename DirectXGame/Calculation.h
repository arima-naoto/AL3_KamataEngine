#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

class Calculation {

public:

	static Vector3 Add(const Vector3& v1, const Vector3& v2);

	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	static Vector3 MultiplyVector(float scalar, const Vector3& v);

	static Matrix4x4 MultiplyMatrix(const Matrix4x4 &m1,const Matrix4x4 &m2);

};