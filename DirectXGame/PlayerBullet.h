#pragma once
#include "WorldTransform.h"

class Model;
class ViewProjection;

class PlayerBullet 
{
public:

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection &viewProjection);


private:

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

};
