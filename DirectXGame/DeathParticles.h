#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "array"

/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticles{

public://メンバ関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewprojection, const Vector3 position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	
	//デスフラグのgetter
	bool GetIsFinished() const { return isFinished_; }

private://メンバ変数

	/// モデルデータ
	Model* model_ = nullptr;

	/// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> worldTransforms_;

	//存続時間(消滅までの時間)<秒>
	static inline const float kDuration = 2.0f;

	//移動の速さ
	static inline const float kSpeed = 0.08f;

	//円周率
	static inline const float pi = 3.14f;

	//分割した1個分の角度
	static inline const float kAngleUnit = (2 * pi) / kNumParticles;

	//終了フラグ
	bool isFinished_ = false;

	//経過時間カウント
	float counter_ = 0.0f;

	//色変更オブジェクト
	ObjectColor objectColor_;

	//色の数値
	Vector4 color_;

};
