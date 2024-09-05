#pragma once
#include "Arithmetic.h"

class Rendering {

public:

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	static Matrix4x4 MakeRotateXMatrix(float radian);
	
	static Matrix4x4 MakeRotateYMatrix(float radian);
	
	static Matrix4x4 MakeRotateZMatrix(float radian);

	static Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

};
