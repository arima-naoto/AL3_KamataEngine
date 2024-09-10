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

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のTx
	worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のTy
	worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のTz

	return worldPos;

}

AABB EnemyBullet::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth_ / 2.0f, worldPos.y - kWidth_ / 2.0f, worldPos.z - kWidth_ / 2.0f};
	aabb.max = {worldPos.x + kHeight_ / 2.0f, worldPos.y + kHeight_ / 2.0f, worldPos.z + kHeight_ / 2.0f};


	return aabb;
}