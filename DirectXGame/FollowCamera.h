#pragma once
#include "WorldTransform.h"
class ViewProjection;
class Input;
class Player;

class FollowCamera {

public:

	void Initialize(ViewProjection *viewprojection);

	void Update();

	void Reset();

	void SetTatget(const WorldTransform* target);

	const ViewProjection* GetViewProjection(){ return viewProjection_; }

private:

	void JoyStickRotation();

	//オフセット計算
	Vector3 CalcOffset() const;

private:

	//ビュープロジェクション
	ViewProjection *viewProjection_ = nullptr;

	//追従対象
	const WorldTransform* target_ = nullptr;

	Input* input_ = nullptr;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	float desticationAngleY = 0.0f;

	Player* player_ = nullptr;

};
