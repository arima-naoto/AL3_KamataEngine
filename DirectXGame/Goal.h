#pragma once
#include "WorldTransform.h"
#include "MyStruct.h"

class Model;
class ViewProjection;
class Player;

class Goal {

public:

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	void OnCollision(Player* player);

	Vector3 GetWorldPosition();

	AABB GetAABB();

private:

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

	static inline const float kwidth = 1.85f;
	static inline const float kheight = 1.85f;


};
