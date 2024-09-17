#include "Enemy.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"
#include "Player.h"
#include "Arithmetic.h"
#include "GameScene.h"

#ifdef _DEBUG
#include "imgui.h"
using namespace ImGui;
#endif // _DEBUG

Enemy::~Enemy() {

	/*for (auto* bullet : bullets_) {
		delete bullet;
	}*/

}

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, uint32_t textureHandle, const Vector3& position) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	textureHandle_ = textureHandle;

	bulletModel_ = Model::CreateFromOBJ("enemyBullet");

	InitalizeApproach();

}

void (Enemy::*Enemy::spFuncTable[])(){

    &Enemy::UpdateApproach, &Enemy::UpdateLeave

};

void Enemy::Update() {


	
	//現在フェーズの関数を実行する
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//行列を更新する
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}

void Enemy::OnCollision() {}

void Enemy::UpdateApproach() {

	worldTransform_.translation_ -= Vector3(0, 0, 0.2f);

	if (worldTransform_.translation_.z < 26.f) {
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

	gameScene_->AddEnemyBullet(newBullet);

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

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth_ / 2.0f, worldPos.y - kWidth_ / 2.0f, worldPos.z - kWidth_ / 2.0f};
	aabb.max = {worldPos.x + kHeight_ / 2.0f, worldPos.y + kHeight_ / 2.0f, worldPos.z + kHeight_ / 2.0f};

	return aabb;
}


//const std::list<EnemyBullet*>& Enemy::GetBullets() const { return bullets_; }