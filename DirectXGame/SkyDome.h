#pragma once
#include "WorldTransform.h"

class Model;
class ViewProjection;

class SkyDome {

public:

	void Initialize(Model *model,ViewProjection*viewProjection);

	void Update();

	void Draw();

private:

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

};
