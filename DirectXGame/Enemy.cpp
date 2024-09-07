#include "Enemy.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"
#include "EnemyBullet.h"

#ifdef _DEBUG
#include "imgui.h"
using namespace ImGui;
#endif // _DEBUG

Enemy::~Enemy() {

	for (auto* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, uint32_t textureHandle) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(10, 0, 50);

	textureHandle_ = textureHandle;

	ChangeState(std::make_unique<EnemyStateApproach>(this));

	bulletModel_ = Model::CreateFromOBJ("enemyBullet", true);
	InitalizeApproach();

}

void Enemy::Update() {

	state_->Update();

	if (phase_ == Phase::Leave && dynamic_cast<EnemyStateLeave*>(state_.get()) == nullptr) {
		ChangeState(std::make_unique<EnemyStateLeave>(this));
	}
	
	for (auto* bullet : bullets_) {
		bullet->Update();
	}

	//行列を更新する
	worldTransform_.UpdateMatrix();

	Begin("enemy");
	DragFloat3("enemy.translate", &worldTransform_.translation_.x, 0.01f);
	End();

}

void Enemy::Draw() {

	for (auto* bullet : bullets_) {
		bullet->Draw(*viewProjection_);
	}

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}

void Enemy::UpdateApproach() {

	worldTransform_.translation_ -= Vector3(0, 0, 0.2f);

	if (worldTransform_.translation_.z < 0.f) {
		phase_ = Phase::Leave;
	}

	if (--fireTimer <= 0) {
		Fire();
		fireTimer = kFireInterval;
	}
}

void Enemy::UpdateLeave() {
	
	worldTransform_.translation_ += Vector3(-0.1f, 0.1f, 0);
	
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) {
	state_ = std::move(state);
}

void Enemy::Fire() {

	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);

}

void Enemy::InitalizeApproach() {

	fireTimer = kFireInterval;

}
