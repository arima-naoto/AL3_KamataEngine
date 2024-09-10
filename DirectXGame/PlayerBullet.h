#pragma once
#include "WorldTransform.h"

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

	Vector3 GetWorldTransform();


private:

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

};
