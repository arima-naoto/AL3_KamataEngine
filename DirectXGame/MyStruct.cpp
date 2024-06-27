#include "MyStruct.h"

bool AABB::IsCollision(const AABB& other) {

	if (this->min.x > other.max.x) {
		return false;
	}
	if (this->max.x < other.min.x) {
		return false;
	}
	if (this->min.y > other.max.y) {
		return false;
	}
	if (this->max.y < other.min.y) {
		return false;
	}
	if (this->min.z > other.max.z) {
		return false;
	}
	if (this->max.z < other.min.z) {
		return false;
	}

	// 衝突していない場合の条件がすべて満たされなければ、衝突している
	return true;

}