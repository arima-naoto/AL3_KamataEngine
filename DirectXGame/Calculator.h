#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#define _USE_MATH_DEFINES
#include "cmath"
#include "cassert"

class Calculator {

public:

	static float Cot(float x);

	static Vector3 Add(const Vector3& v1, const Vector3& v2);

	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	static Vector3 Multiply(float s, const Vector3& v);

	static Vector3 Forward(const Vector3& v, float s);

	static float Length(const Vector3& v);

	static Vector3 Normalize(const Vector3& v);

	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
	
	static Matrix4x4 Multiply (const Matrix4x4& m1, const Matrix4x4& m2);

	static float LerpShortAngle(float a, float b, float t);

};
