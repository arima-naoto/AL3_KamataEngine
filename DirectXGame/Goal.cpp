#include "Goal.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

void Goal::Initialize(Model *model,ViewProjection *viewProjection,const Vector3 &position) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

}

void Goal::Update() {

	worldTransform_.UpdateMatrix();

}

void Goal::Draw() {

	model_->Draw(worldTransform_, *viewProjection_);

}

void Goal::OnCollision(Player* player) {

	(void)player;

	worldTransform_.rotation_ += {0, 0.5f, 0};

}

Vector3 Goal::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のTx
	worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のTy
	worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のTz

	return worldPos;

}

AABB Goal::GetAABB() {

	Vector3 worldPos = Goal::GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kwidth / 2.0f, worldPos.y - kheight / 2.0f, worldPos.z - kwidth / 2.0f};
	aabb.max = {worldPos.x + kwidth / 2.0f, worldPos.y + kheight / 2.0f, worldPos.z + kwidth / 2.0f};

	return aabb;

}

