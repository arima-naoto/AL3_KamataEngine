#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
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