#include "Arithmetic.h"

Vector2 operator-(const Vector2& v1, const Vector2& v2) { return Calculator::Subtract(v1, v2); }

Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Calculator::Add(v1, v2);}

Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Calculator::Subtract(v1, v2);}

Vector3 operator*(float s, const Vector3& v) { return Calculator::Multiply(s, v);}

Vector3 operator*(const Vector3& v, float s) { return s * v;}

Vector3 operator/(const Vector3& v, float s) { return Calculator::Forward(v, s); }

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return Calculator::Add(m1, m2); }

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) { return Calculator::Subtract(m1, m2); }

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2){ return Calculator::Multiply(m1, m2);}

Vector3 operator-(const Vector3& v) { return Vector3(-v.x, -v.y, -v.z); }