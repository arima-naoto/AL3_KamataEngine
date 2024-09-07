#include "EnemyBullet.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"
#include "EnemyBullet.h"
#include "Arithmetic.h"
#include "Player.h"
#include "Enemy.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;
	worldTransform_.scale_ = Vector3(0.5f, 0.5f, 3.0f);
	Angularization(velocity);

}

void EnemyBullet::Update() {

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ += velocity_;

	if (t < 1.0f) {
		t += 0.045f / 60.0f;
	} else {
		t = 1.0f;
	}

	Vector3 toPlayer = player_->GetWorldPosition() - GetWorldPosition();

	Calculation::Normalize(toPlayer);
	Calculation::Normalize(velocity_);

	velocity_ = Calculation::Slerp(velocity_, toPlayer, t) * Enemy::kBulletSpeed;
	Angularization(velocity_);

	worldTransform_.UpdateMatrix();

}

void EnemyBullet::Draw(const ViewProjection &viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}

void EnemyBullet::Angularization(const Vector3& velocity) {

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float velocityXZ = Calculation::Length({velocity.x, 0.0f, velocity.z});
	// 横軸方向の長さを求める
	worldTransform_.rotation_.x = std::atan2(-velocity.y, velocityXZ);

}

Vector3 EnemyBullet::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のTx
	worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のTy
	worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のTz

	return worldPos;

}