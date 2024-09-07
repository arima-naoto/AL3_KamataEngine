#include "EnemyBullet.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"
#include "EnemyBullet.h"
#include "Calculation.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;
	worldTransform_.scale_ = Vector3(0.5f, 0.5f, 3.0f);

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float velocityXZ = Calculation::Length({velocity.x, 0.0f, velocity.z});
	//横軸方向の長さを求める
	worldTransform_.rotation_.x = std::atan2(-velocity.y, velocityXZ);

}

void EnemyBullet::Update() {

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

}

void EnemyBullet::Draw(const ViewProjection &viewProjection) {


	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	model_->Draw(worldTransform_, viewProjection);

}