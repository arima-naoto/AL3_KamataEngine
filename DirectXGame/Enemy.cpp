#include "Enemy.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"
#include "EnemyBullet.h"
#include "Player.h"
#include "Arithmetic.h"

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

	bulletModel_ = Model::CreateFromOBJ("enemyBullet");

	InitalizeApproach();

}



void Enemy::Update() {
	
	//現在フェーズの関数を実行する
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

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

void Enemy::OnCollision() {}

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

void (Enemy::*Enemy::spFuncTable[])(){

    &Enemy::UpdateApproach,
	&Enemy::UpdateLeave

};

void Enemy::Fire() {

	assert(player_);

	const float kBulletSpeed = 1.0f;

	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = GetWorldPosition();
	Vector3 SubtractVector = playerPos - enemyPos;
	Vector3 NomalizeVector = Calculation::Normalize(SubtractVector);
	Vector3 velocity(NomalizeVector * kBulletSpeed);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);

}

void Enemy::InitalizeApproach() {

	fireTimer = kFireInterval;

}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のTx
	worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のTy
	worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のTz

	return worldPos;

}

const std::list<EnemyBullet*>& Enemy::GetBullets() const { return bullets_; }