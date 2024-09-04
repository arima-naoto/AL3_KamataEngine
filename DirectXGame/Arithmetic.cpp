#include "Arithmetic.h"
#include "Calculation.h"

Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Calculation::Add(v1, v2); }

Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Calculation::Subtract(v1, v2); }

Vector3 operator*(float s, const Vector3& v) { return Calculation::MultiplyVector(s, v); }

Vector3 operator*(const Vector3& v, float s) { return s * v; }

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return Calculation::MultiplyMatrix(m1, m2); }