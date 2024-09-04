#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"

void Player::Initialize(Model* model, ViewProjection * viewProjection,uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;//モデル
	textureHandle_ = textureHandle;//テクスチャハンドル

	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	
}

void Player::Update() 
{
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() 
{ 
	//3Dモデルを描画
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_); 
}