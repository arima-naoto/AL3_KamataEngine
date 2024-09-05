#pragma once
#include "WorldTransform.h"

class Model;
class ViewProjection;

enum class Phase {
	Approach,
	Leave,
};

class Enemy {

public:

	void Initialize(Model* model,ViewProjection *viewProjection,uint32_t textureHandle);

	void Update();

	void Draw();

private:

	void UpdateApproach();

	void UpdateLeave();

private:

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;
	static void (Enemy::*spFuncTable[])();

};
