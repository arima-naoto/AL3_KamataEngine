#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"


/// <summary>
/// ヒットエフェクト
/// </summary>
class HitEffect {

public:

	//コンストラクタ
	HitEffect() = default;
	//デストラクタ
	~HitEffect() = default;
	//初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position);
	//更新
	void Update();
	//描画
	void Draw();

	void DrawDebugText();

private://メンバ変数

	//モデルデータ
	Model* model_ = nullptr;
	//ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//存続時間(消滅までの時間)<秒>
	static inline const float kDuration = 0.3f;
	//移動の速さ
	static inline const float kSpeed = 0.07f;
	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;
	//色変更オブジェクト
	ObjectColor objectColor_;
	//色の数値
	Vector4 color_;
};