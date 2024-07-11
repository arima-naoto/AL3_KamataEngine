#include "Bar.h"
#include "cassert"

///タイトルバーの初期化処理
void Bar::Initialize(Model* model, ViewProjection* viewprojection) {

	assert(model);

	model_ = model;

	viewProjection_ = viewprojection;

	worldTransform_.Initialize();

}

///タイトルバーの初期化処理
void Bar::Update() {}

///タイトルバーの描画処理
void Bar::Draw() {

	model_->Draw(worldTransform_, *viewProjection_);

}

