#define NOMINMAX
#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "imgui.h"
#include "algorithm"

void Player::Initialize(Model* model, ViewProjection * viewProjection,uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;//モデル

	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	
	//ワールド変換の初期化
	worldTransform_.Initialize();

	textureHandle_ = textureHandle; // テクスチャハンドル

	input_ = Input::GetInstance();

}

void Player::Update() 
{

	const float kLimitMoveX = 33;
	const float kLimitMoveY = 18;

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kLimitMoveX, kLimitMoveX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kLimitMoveY, kLimitMoveY);

	//行列を更新する
	worldTransform_.UpdateMatrix();
}

void Player::Draw() 
{ 
	//3Dモデルを描画
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_); 

}

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