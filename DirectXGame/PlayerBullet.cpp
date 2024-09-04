#include "PlayerBullet.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

}

void PlayerBullet::Update() {

	worldTransform_.UpdateMatrix();

}

void PlayerBullet::Draw(const ViewProjection &viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}