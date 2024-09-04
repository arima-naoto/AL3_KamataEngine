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

	//プレイヤーの移動
	MoveTranslate();

	worldTransform_.translation_ = 

	//行列を更新する
	worldTransform_.UpdateMatrix();
}

void Player::Draw() 
{ 
	//3Dモデルを描画
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_); 

}

void Player::MoveTranslate() {

	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) 
	{
		move += Vector3(-kCharacterSpeed,0,0);
	} 
	else if (input_->PushKey(DIK_RIGHT)) 
	{
		move += Vector3(kCharacterSpeed, 0, 0);
	} 
	else if (input_->PushKey(DIK_UP)) 
	{
		move += Vector3(0,kCharacterSpeed, 0);
	} 
	else if (input_->PushKey(DIK_DOWN)) 
	{
		move += Vector3(0, -kCharacterSpeed, 0);
	}

	worldTransform_.translation_ += move;

}