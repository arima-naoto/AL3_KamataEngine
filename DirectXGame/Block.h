#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <vector>

class Block 
{
public:

	void Initialize(Model* model,ViewProjection *viewProjection);

	void Update();

	void Draw(const WorldTransform& worldTransform);

private:

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

};
