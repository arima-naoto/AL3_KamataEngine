#pragma once
#include "project/math/Arithmetic.h"


class Rendering : public Calculator
{
public:

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	static Matrix4x4 MakeRotateXMatrix(float theta);

	static Matrix4x4 MakeRotateYMatrix(float theta);
	
	static Matrix4x4 MakeRotateZMatrix(float theta);

	static Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	static Matrix4x4 SRTAffineMatrix(const Affine& affine);
	
	static Matrix4x4 STRAffineMatrix(const Affine& affine);

	static Matrix4x4 Inverse(const Matrix4x4& m);

	static Matrix4x4 MakePerspectiveFovMatrix(Fov<float> perspective);

	static Matrix4x4 ViewportMatrix(ViewRect<float> viewport);

	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

};

