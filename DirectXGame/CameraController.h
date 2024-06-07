#pragma once
#include "ViewProjection.h"
//前方宣言
class Player;

/// <summary>
/// 短形の構造体
/// </summary>
struct Rect {
	float left = 0.0f;   // 左端
	float right = 1.0f;  // 右端
	float bottom = 0.0f; // 下端
	float top = 1.0f;    // 上端
};

/// <summary>
/// Lerp関数の宣言
/// </summary>
Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

/// <summary>
/// カメラコントローラクラスの作成
/// </summary>
class CameraController 
{
public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CameraController();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CameraController();

	/// <summary>
	/// 初期化メンバ関数
	/// </summary>
	void Initialize();

	/// <summary>
	/// リセットメンバ関数
	/// </summary>
	void Reset();

	/// <summary>
	/// 更新メンバ関数
	/// </summary>
	void Update();

public://セッター

	/// <summary>
	/// 追従対象のセッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(Player* target) { target_ = target; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void SetMovableArea(const Rect& area) { movableArea_ = area; }

private://メンバ変数

	
	/// <summary>
	/// ビュープロジェクション
	/// </summary>
	ViewProjection viewProjection_;

	/// <summary>
	/// 追従対象
	/// </summary>
	Player* target_ = nullptr;

	/// <summary>
	/// 追従対象とカメラの座標の差(オフセット)
	/// </summary>
	Vector3 targetOffset_ = {0, 0, -30.0f};

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	//追従対象の各方向へのカメラ移動範囲
	Rect margin_ = {-5, 10, -5, 10};

	// カメラの目標座標
	Vector3 targetPosition_;

	// 座標補間割合
	static inline const float kIntrepolationRate = 0.9f;

	//速度掛け率
	static inline const float kVelocityBias_ = 15.0f;

};
