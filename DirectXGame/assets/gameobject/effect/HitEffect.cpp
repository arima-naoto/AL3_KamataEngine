#include "HitEffect.h"
#include "cassert"
#include "assets/process/math/Rendering.h"
#include "algorithm"
#include"imgui.h"

void HitEffect::Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position) {
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void HitEffect::Update() {

	if (isFinished_) {
		return;
	}

	

	// ワールド変換データの更新
	worldTransform_.UpdateMatrix();

	Vector3 velocity = {kSpeed,kSpeed,0};
	// 移動処理
	worldTransform_.scale_ += velocity;

	// カウンターを一フレーム文の秒数を進める
	counter_ += 1.0f / 60.0f;

	// 存続時間の条件に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}

	color_.w = std::clamp(1.0f - (counter_ / kDuration), 0.0f, 1.0f);

	// 色変更オブジェクトに色に数値を設定する
	objectColor_.SetColor(color_);

	// 色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();
}

void HitEffect::Draw() {

	//終了なら何もしない
	if (isFinished_) {
		return;
	}

	//モデルの描画
	model_->Draw(worldTransform_, *viewProjection_, &objectColor_);

}

void HitEffect::DrawDebugText() { ImGui::DragFloat3("hitEffect translate", &worldTransform_.translation_.x, 0.01f); }


