#pragma once

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3& operator+=(const Vector3& obj) {
		x += obj.x;
		y += obj.y;
		z += obj.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& obj) {
		x -= obj.x;
		y -= obj.y;
		z -= obj.z;
		return *this;
	}
};

struct Matrix4x4 {

	float m[4][4];

};

class Calculation {

public:

	static Vector3 Add(const Vector3& v1, const Vector3& v2);

	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	static Vector3 MultiplyVector(float scalar, const Vector3& v);

	static Matrix4x4 MultiplyMatrix(const Matrix4x4 &m1,const Matrix4x4 &m2);

};