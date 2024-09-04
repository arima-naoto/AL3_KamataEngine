#include "Rendering.h"
#define _USE_MATH_DEFINES
#include "cmath"

Matrix4x4 Rendering::MakeScaleMatrix(const Vector3& scale)
{

	return {
		scale.x,0.0f,0.0f,0.0f,
		0.0f,scale.y,0.0f,0.0f,
		0.0f,0.0f,scale.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

}

Matrix4x4 Rendering::MakeRotateXMatrix(float radian)
{
	return {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cosf(radian),sinf(radian),0.0f,
		0.0f,-sinf(radian),cosf(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4x4 Rendering::MakeRotateYMatrix(float radian)
{
	return {
		cosf(radian),0.0f,-sinf(radian),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sinf(radian),0.0f,cosf(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4x4 Rendering::MakeRotateZMatrix(float radian)
{
	return {
		cosf(radian),sinf(radian),0.0f,0.0f,
		-sinf(radian),cosf(radian),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4x4 Rendering::MakeRotateMatrix(const Vector3& rotate) 
{
	Matrix4x4 rotateXMatrix = Rendering::MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = Rendering::MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = Rendering::MakeRotateZMatrix(rotate.z);

	Matrix4x4 rotateMatrix = rotateZMatrix * rotateXMatrix * rotateYMatrix;

	return rotateMatrix;
}

Matrix4x4 Rendering::MakeTranslateMatrix(const Vector3 &translate)
{
	return {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate.x,translate.y,translate.z,1.0f
	};
}

Matrix4x4 Rendering::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 scaleMatrix = Rendering::MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = Rendering::MakeRotateMatrix(rotate);
	Matrix4x4 translateMatrix = Rendering::MakeTranslateMatrix(translate);

	Matrix4x4 affineMatrix = scaleMatrix * rotateMatrix * translateMatrix;

	return affineMatrix;
}