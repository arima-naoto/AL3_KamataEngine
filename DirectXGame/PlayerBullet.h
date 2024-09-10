#pragma once
#include "WorldTransform.h"
#include "Calculation.h"

class Model;
class ViewProjection;

class PlayerBullet 
{
public:

	void Initialize(Model* model, const Vector3& position,const Vector3 &velocity);

	void Update();

	void Draw(const ViewProjection &viewProjection);

	void OnCollision();

	bool GetIsDead() const { return this->isDead_; }

	Vector3 GetWorldPosition();

	AABB GetAABB();

private:

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

	static inline const float kWidth_ = 2.0f;
	static inline const float kHeight_ = 2.0f;

};
