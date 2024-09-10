#include "EnemyBullet.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;

}

void EnemyBullet::Update() {

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

}

void EnemyBullet::Draw(const ViewProjection &viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}

void EnemyBullet::OnCollision() {
	isDead_ = true;
}

