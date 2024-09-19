#pragma once
#include "WorldTransform.h"
class ViewProjection;
class Input;

class FollowCamera {

public:

	void Initialize(ViewProjection *viewprojection);

	void Update();

	void SetTatget(const WorldTransform* target) { target_ = target; }

	const ViewProjection* GetViewProjection(){ return viewProjection_; }

private:

	void JoyStickRotation();

private:

	//ビュープロジェクション
	ViewProjection *viewProjection_ = nullptr;

	//追従対象
	const WorldTransform* target_ = nullptr;

	Input* input_ = nullptr;

};
