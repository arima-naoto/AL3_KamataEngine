#include "Calculator.h"
#include "Arithmetic.h"

float Calculator::Cot(float x) { return 1.0f / tanf(x); }

Vector3 Calculator::Add(const Vector3& v1, const Vector3& v2) { 
	Vector3 resultAdd;
	resultAdd.x = v1.x + v2.x;
	resultAdd.y = v1.y + v2.y;
	resultAdd.z = v1.z + v2.z;
	return resultAdd;
}

Vector3 Calculator::Subtract(const Vector3& v1, const Vector3& v2) { 
	Vector3 resultSubract;
	resultSubract.x = v1.x - v2.x;
	resultSubract.y = v1.y - v2.y;
	resultSubract.z = v1.z - v2.z;
	return resultSubract;
}

Vector3 Calculator::Multiply(float s, const Vector3& v) {
	Vector3 resultMultiply;
	resultMultiply.x = s * v.x;
	resultMultiply.y = s * v.y;
	resultMultiply.z = s * v.z;
	return resultMultiply;
}

Vector3 Calculator::Forward(const Vector3& v, float s) {

	Vector3 resultForward;
	resultForward.x = 1.0f / s * v.x;
	resultForward.y = 1.0f / s * v.y;
	resultForward.z = 1.0f / s * v.z;
	return resultForward;
}

float Calculator::Length(const Vector3& v) {

	float scalarX = std::powf(v.x, 2);
	float scalarY = std::powf(v.y, 2);
	float scalarZ = std::powf(v.z, 2);

	return std::sqrtf(scalarX + scalarY + scalarZ);
}

Vector3 Calculator::Normalize(const Vector3& v) {

	// 正規化を使用して計算結果を求める
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	Vector3 resultNormalize = {v.x, v.y, v.z};

	if (length != 0.0f) {
		resultNormalize = {v.x / length, v.y / length, v.z / length};
	}
	return resultNormalize;

}

Matrix4x4 Calculator::Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultAdd{};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			resultAdd.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}

	return resultAdd;
}

Matrix4x4 Calculator::Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 subtract{};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			subtract.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return subtract;
}

Matrix4x4 Calculator::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 multiply{};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			multiply.m[i][j] = 0;
			for (int k = 0; k < 4; k++)
			{
				multiply.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return multiply;
}

Vector3 Calculator::Lerp(const Vector3& a, const Vector3& b, float t) { 
	return Vector3(a * (1.0f - t) + b * t); }

float Calculator::LerpShortAngle(float a, float b, float t) {

	// aとbの間の差を計算し,正規化する
	float diff = fmod(b - a + float(M_PI), 2 * float(M_PI)) - float(M_PI);

	// 線形補間を適用する
	return a + diff * t;
}