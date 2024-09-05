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

	ChangeState(std::make_unique<EnemyStateApproach>(this));

}

void Enemy::Update() {

	state_->Update();

	if (phase_ == Phase::Leave && dynamic_cast<EnemyStateLeave*>(state_.get()) == nullptr) {
		ChangeState(std::make_unique<EnemyStateLeave>(this));
	}
	
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

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> stateApproach) {
	state_ = std::move(stateApproach);
}