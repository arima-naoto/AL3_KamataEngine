#pragma once
#include "WorldTransform.h"
class Model;
class ViewProjection;
class Player;

class EnemyBullet {

public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool GetIsDead() const { return this->isDead_; }

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

private:

	void Angularization(const Vector3& velocity);
	
private:

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

	Player* player_ = nullptr;

	float t = 0.0f;

};
