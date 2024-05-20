#include "Player.h"

void Player::Initialize(Model* model,ViewProjection * viewProjection) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;//モデル

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	
}

void Player::Update() 
{
	//行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection &viewProjection) 
{ 
	//3Dモデルを描画
	model_->Draw(worldTransform_,viewProjection); 
}