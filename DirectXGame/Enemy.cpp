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
	
	//現在フェーズの関数を実行する
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//行列を更新する
	worldTransform_.UpdateMatrix();

	Begin("enemy");
	DragFloat3("enemy.translate", &worldTransform_.translation_.x, 0.01f);
	End();

}

void Enemy::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}

void Enemy::UpdateApproach() {

	worldTransform_.translation_ -= Vector3(0, 0, 0.2f);

	if (worldTransform_.translation_.z < 0.f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave() {
	
	worldTransform_.translation_ += Vector3(-0.1f, 0.1f, 0);
	
}

void (Enemy::*Enemy::spFuncTable[])(){

    &Enemy::UpdateApproach,
	&Enemy::UpdateLeave

};