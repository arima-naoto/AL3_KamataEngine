#pragma once
#include "WorldTransform.h"
#include "EnemyState.h"
#include "memory"
#include "list"

class Model;
class ViewProjection;
class EnemyBullet;
class TimedCall;

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

	void UpdateApproach();

	void UpdateLeave();

	void Launch_And_Reset();

private:

	void ChangeState(std::unique_ptr<BaseEnemyState> state);

	void Fire();

	void InitalizeApproach();

private:

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;

	std::unique_ptr<BaseEnemyState> state_;

	Model* bulletModel_ = nullptr;
	std::list<EnemyBullet*> bullets_;

	static const int kFireInterval = 60;
	int32_t fireTimer = 0;

	std::list<TimedCall*> timedCalls_;

};
