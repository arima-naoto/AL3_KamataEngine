#pragma once
#include "Vector3.h"

/// <summary>
/// 軸並行境界箱構造体を宣言
/// </summary>
struct AABB {
	Vector3 min;//最小点
	Vector3 max;//最大点

};