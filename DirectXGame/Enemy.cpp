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
void Enemy::Initialize(std::vector<Model*> models, ViewProjection* viewProjection) {

	//NULLポインタチェック

	// 引数で受け取ったデータをメンバ変数に記録する
	models_ = models;
	viewProjection_ = viewProjection;

	InitializeWorldTransform();
}

/// 更新処理
void Enemy::Update() {

	//向いている方向に移動
	Enemy::Move();
	UpdateFloatingGimmick();
	DragFloat3("L_Spear.translate", &worldTransforms_[2]->translation_.x, 0.01f);

	//行列の更新
	for (auto *worldTransform : worldTransforms_) {
		worldTransform->UpdateMatrix();
	}

}

/// 描画処理
void Enemy::Draw() {

	///3Dモデルを描画する
	models_[1]->Draw(*worldTransforms_[1], *viewProjection_);
	models_[2]->Draw(*worldTransforms_[2], *viewProjection_);
	models_[3]->Draw(*worldTransforms_[3], *viewProjection_);

}

void Enemy::InitializeWorldTransform() {

	for (int i = 0; i < 4; i++) {
		worldTransforms_.resize(4);
		WorldTransform* worldTransform = new WorldTransform();
		worldTransform->Initialize();
		worldTransforms_[i] = worldTransform;
	}

	
	worldTransforms_[0]->rotation_.y = pi_v<float> / -2.0f;
	worldTransforms_[0]->translation_ = {0.f, -0.65f, 10.0f};

	worldTransforms_[1]->parent_ = this->GetWorldTransform()[0];
	worldTransforms_[2]->parent_ = this->GetWorldTransform()[1];
	worldTransforms_[2]->translation_.x = 0.87f;
	worldTransforms_[3]->parent_ = this->GetWorldTransform()[1];
	worldTransforms_[3]->translation_.x = -0.87f;

}

void Enemy::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Enemy::Move() {

	worldTransforms_[0]->rotation_.y += 1.0f / 90.0f;

	const float kCharacterSpeed = 0.1f;
	velocity_ = {0, 0, kCharacterSpeed};

	velocity_ = Rendering::TransformNormal(velocity_, worldTransforms_[0]->matWorld_);

	worldTransforms_[0]->translation_ += velocity_;

}

void Enemy::UpdateFloatingGimmick() {

	const int32_t cycle = 90;

	// 1フレーム出のパラメータ加算値
	const float step = pi_v<float> * 2.0f / cycle;
	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2π超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, pi_v<float> * 2.0f);
	const float amplitube = 0.2f;
	// 浮遊を座標に反映
	worldTransforms_[2]->translation_.z = std::sin(floatingParameter_) * amplitube;
	worldTransforms_[3]->translation_.z = std::sin(floatingParameter_) * -amplitube;
	


}