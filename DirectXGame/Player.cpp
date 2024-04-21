#include "Player.h"
#include "cassert"

void Player::Initialize(Model* model, uint32_t textureHandle) { 

	//NULLポインタチェック
	assert(model); 

	//引数として受け取ったデータをメンバ変数に記録する
	textureHandle_ = textureHandle;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

void Player::Update() {}

void Player::Draw() {}