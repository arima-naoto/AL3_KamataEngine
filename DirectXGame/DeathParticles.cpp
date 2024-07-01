#include "DeathParticles.h"
#include "cassert"
#include "WorldTransform.h"
#include "Model.h"
#include "Rendering.h"
#include "algorithm"

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="model">モデルデータ</param>
/// <param name="viewProjection">ビュープロジェクション行列</param>
/// <param name="position">座標</param>
void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position) {

	///NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	//ワールド変数の初期化
	for (auto& WorldTransform : worldTransforms_) {

		WorldTransform.Initialize();
		WorldTransform.translation_ = position;

	}

	//引数の内容をメンバ変数に記録する
	viewProjection_ = viewProjection;

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

}

/// <summary>
/// 更新処理
/// </summary>
void DeathParticles::Update() {

	//終了なら何もしない
	if (isFinished_) {
		return;
	}

	//ワールド変換の更新
	for (auto& WorldTransform : worldTransforms_) {
		WorldTransform.UpdateMatrix();
	}

#pragma region パーティクルの移動

	for (uint32_t i = 0; i < kNumParticles; i++) {
		//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		//回転角を計算する
		float angle = kAngleUnit * i;
		//Z周りの回転行列
		Matrix4x4 rotateMatrixZ = Rendering::MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて、速度ベクトルを得る
		velocity = Rendering::Transform(velocity, rotateMatrixZ);
		//移動処理
		worldTransforms_[i].translation_ += velocity;
	}

#pragma endregion


#pragma region 一定の存続時間に達したらパーティクルを消す処理

	//カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	//存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		//終了扱いにする
		isFinished_ = true;
	}

#pragma endregion

	color_.w = std::clamp(1.0f - (counter_ / kDuration), 0.0f, 1.0f);

	//色変更オブジェクトに色に数値を設定する
	objectColor_.SetColor(color_);
	//色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();

}

void DeathParticles::Draw() {

	//終了なら何もしない
	if (isFinished_) {
		return;
	}

	//モデルの描画
	for (auto& WorldTransform : worldTransforms_) {

		model_->Draw(WorldTransform,*viewProjection_,&objectColor_);
	}

}