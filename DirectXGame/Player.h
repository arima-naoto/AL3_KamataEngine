#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include "ViewProjection.h"
#include "Vector3.h"

enum class LRDirection {
	kRight,
	kLeft,
};

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public://メンバ関数

	/// <summary>
	/// インアウトサイン
	/// </summary>
	/// <param name="easing"></param>
	/// <returns></returns>
	float EaseInOutSine(float easing);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, ViewProjection* viewProjection,const Vector3&position);

	/// <summary>
	/// 右の減速メンバ関数
	/// </summary>
	void IsRightDecelerate();

	/// <summary>
	/// 左の減速メンバ関数
	/// </summary>
	void IsLeftDecelerate();

	/// <summary>
	/// 右の振り向きメンバ関数
	/// </summary>
	void RightDirection();

	/// <summary>
	/// 左の振り向きメンバ関数
	/// </summary>
	void LeftDirection();

	/// <summary>
	/// 旋回制御メンバ関数
	/// </summary>
	void TurningControl();

	/// <summary>
	/// ジャンプ制御メンバ関数
	/// </summary>
	void JumpControl();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private://メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//ビュープロジェクション
	ViewProjection *viewProjection_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.01f;

	//移動減衰
	static inline const float kAttenuation = 0.2f;

	//最大速度制限
	static inline const float kLimitRunSpeed = 0.3f;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回時間
	static inline const float kTimeTurn = 0.3f;

	// 接地状態フラグ
	bool onGround_ = true;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.5f;

	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.3f;

	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 2.5f ;

};
