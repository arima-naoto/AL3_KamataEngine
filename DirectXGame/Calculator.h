#pragma once
#include "project/math/Structure.h"
#define _USE_MATH_DEFINES
#include "cmath"
#include "cassert"

class Calculator {

public:

	static float Cot(float x);

	static Vector2 Subtract(const Vector2& v1, const Vector2& v2);

	static Vector3 Add(const Vector3& v1, const Vector3& v2);

	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	static Vector3 Multiply(float s, const Vector3& v);

	static Vector3 Forward(const Vector3& v, float s);

	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
	
	static Matrix4x4 Multiply (const Matrix4x4& m1, const Matrix4x4& m2);

};
