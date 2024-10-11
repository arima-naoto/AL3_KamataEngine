#include "Enemy.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"
#include "numbers"
using namespace std::numbers;
#include "Rendering.h"

#ifdef _DEBUG
#include "imgui.h"
using namespace ImGui;
#endif // _DEBUG


///初期化処理
void Enemy::Initialize(Model* model, ViewProjection* viewProjection) {

	//NULLポインタチェック
	assert(model);

	// 引数で受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.rotation_.y = pi_v<float> / -2.0f;
	worldTransform_.translation_ = {0.f, -0.65f, 10.0f};

}

/// 更新処理
void Enemy::Update() {

	//向いている方向に移動
	Enemy::Move();

#ifdef _DEBUG
	DragFloat3("enemy.rotate", &worldTransform_.rotation_.x, 0.01f);
	DragFloat3("enemy.translate", &worldTransform_.translation_.x, 0.01f);
#endif // _DEBUG

	//行列の更新
	worldTransform_.UpdateMatrix();

}

/// 描画処理
void Enemy::Draw() {

	///3Dモデルを描画する
	model_->Draw(worldTransform_, *viewProjection_);

}

void Enemy::Move() {

	worldTransform_.rotation_.y += 1.0f / 90.0f;

	const float kCharacterSpeed = 0.1f;
	velocity_ = {0, 0, kCharacterSpeed};

	velocity_ = Rendering::TransformNormal(velocity_, worldTransform_.matWorld_);

	worldTransform_.translation_ += velocity_;

}