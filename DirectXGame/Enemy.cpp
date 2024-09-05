#include "Enemy.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

#ifdef _DEBUG
#include "imgui.h"
using namespace ImGui;
#endif // _DEBUG

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, uint32_t textureHandle) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0, 2, 30);

	textureHandle_ = textureHandle;

}

void Enemy::Update() {

	switch (phase_) {
	case Phase::Approach:
		
     	worldTransform_.translation_ -= Vector3(0, 0, 0.2f);

		if (worldTransform_.translation_.z < 0.f) {
			phase_ = Phase::Leave;
		}
		
		break;
	case Phase::Leave:

		worldTransform_.translation_ += Vector3(-0.1f, 0.1f, 0);

		break;
	}


	Begin("enemy");
	DragFloat3("enemy.translate", &worldTransform_.translation_.x, 0.01f);
	End();


	worldTransform_.UpdateMatrix();

}

void Enemy::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}