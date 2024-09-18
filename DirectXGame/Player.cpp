#include "Player.h"

void Player::Initialize(Model* modelBody, Model* modelHead,Model* modelL_arm, 
	Model* modelR_arm, ViewProjection* viewProjection) {

	//NULLポインタチェック
	assert(modelBody, modelHead, modelL_arm, modelR_arm);

	//引数として受け取ったデータをメンバ変数に記録する
	modelBody_ = modelBody;//モデル

	//ワールド変換の初期化
	worldTransformBase_.Initialize();

	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	
}

void Player::Update() 
{
	//行列を定数バッファに転送
	worldTransformBase_.TransferMatrix();
}

void Player::Draw() 
{ 
	//3Dモデルを描画
	modelBody_->Draw(worldTransformBase_,*viewProjection_); 
}