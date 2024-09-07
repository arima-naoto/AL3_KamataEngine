#pragma once
#include "WorldTransform.h"
#include "list"

class Model;
class ViewProjection;
class EnemyBullet;
class Player;

enum class Phase {
	Approach,
	Leave,
};

class Enemy {

public:

	~Enemy();

	void Initialize(Model* model,ViewProjection *viewProjection,uint32_t textureHandle);

	void Update();

	void Draw();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition(); 

	static inline const float kBulletSpeed = 1.0f;

private:

	void UpdateApproach();

	void UpdateLeave();

	void Fire();

	void InitalizeApproach();

private:

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;
	static void (Enemy::*spFuncTable[])();

	Model* bulletModel_ = nullptr;
	std::list<EnemyBullet*> bullets_; 

	static const int kFireInterval = 60;
	int32_t fireTimer = 0;

	Player* player_ = nullptr;

};
