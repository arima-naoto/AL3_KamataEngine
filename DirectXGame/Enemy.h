#pragma once
#include "WorldTransform.h"

class Model;
class ViewProjection;

class Enemy {

public:

	void Initialize(Model* model,ViewProjection *viewProjection,uint32_t textureHandle);

	void Update();

	void Draw();

private:

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

};
