#include "SkyDome.h"
#include "cassert"

/// <summary>
/// 初期化処理
/// </summary>
void SkyDome::Initialize(Model *model,ViewProjection *viewProjection) 
{
	//NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void SkyDome::Update() {

	worldTransform_.rotation_ += Vector3(0, 0.001f, 0);

	worldTransform_.UpdateMatrix();

}

void SkyDome::Draw() 
{
	//3Dモデルの描画
	model_->Draw(worldTransform_,*viewProjection_);
}