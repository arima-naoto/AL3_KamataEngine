#define NOMINMAX
#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "imgui.h"
#include "algorithm"
#include "PlayerBullet.h"

void Player::Initialize(Model* model, ViewProjection * viewProjection,uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;//モデル

	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	
	//ワールド変換の初期化
	worldTransform_.Initialize();

	// テクスチャハンドル
	textureHandle_ = textureHandle; 
	
	input_ = Input::GetInstance();

	modelBullet_ = Model::CreateFromOBJ("playerBullet", true);
}

void Player::Update() 
{

	// プレイヤーの移動範囲を設定する
	const float kLimitMoveX = 33;
	const float kLimitMoveY = 18;

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kLimitMoveX, kLimitMoveX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kLimitMoveY, kLimitMoveY);

	Player::Attack();

	if (bullet_) {
		bullet_->Update();
	}

	//行列を更新する
	worldTransform_.UpdateMatrix();
}

void Player::Draw() 
{ 

	if (bullet_) {
		bullet_->Draw(*viewProjection_);
	}

	//3Dモデルを描画
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_); 

}

#pragma region 移動処理メンバ関数の定義

void Player::MoveRight() {
	worldTransform_.translation_ += Vector3(kCharacterSpeed, 0, 0);
}

void Player::MoveLeft() {
	worldTransform_.translation_ -= Vector3(kCharacterSpeed, 0, 0);
}

void Player::MoveUp() { 
	worldTransform_.translation_ += Vector3(0, kCharacterSpeed, 0); 
}

void Player::MoveDown() { 
	worldTransform_.translation_ -= Vector3(0, kCharacterSpeed, 0); 
}

#pragma endregion

#pragma region 回転処理メンバ関数の定義

void Player::RotateRight() {
	worldTransform_.rotation_ -= Vector3(0, kRotSpeed, 0);
}

void Player::RotateLeft() {
	worldTransform_.rotation_+= Vector3(0, kRotSpeed, 0);
}

#pragma endregion 

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(modelBullet_, worldTransform_.translation_);

		bullet_ = newBullet;
	}
}