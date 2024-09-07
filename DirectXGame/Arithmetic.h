#pragma once
#include "Calculation.h"

Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3 operator*(float s, const Vector3& v);

Vector3 operator*(const Vector3& v, float s);

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 operator*=(Vector3& v, float s);


