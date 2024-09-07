#pragma once
#include "WorldTransform.h"
#include "EnemyState.h"
#include "memory"

class Model;
class ViewProjection;
class EnemyBullet;

enum class Phase {
	Approach,
	Leave,
};

class Enemy {

public:

	void Initialize(Model* model,ViewProjection *viewProjection,uint32_t textureHandle);

	void Update();

	void Draw();

	void UpdateApproach();

	void UpdateLeave();

	void ChangeState(std::unique_ptr<BaseEnemyState> state);

	void Fire();

private:

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;

	std::unique_ptr<BaseEnemyState> state_;

	std::list<EnemyBullet*> bullets_;

};
