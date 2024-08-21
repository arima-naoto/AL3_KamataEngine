#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "cassert"
#include "ViewProjection.h"
#include "Vector3.h"
#include "MyStruct.h"

///Enemyクラスの前方宣言
class Enemy;

/// <summary>
/// マップチップクラスの前方宣言
/// </summary>
class MapChipField;

enum class LRDirection {
	kRight,
	kLeft,
};

/// <summary>
/// マップチップの当たり判定情報
/// </summary>
struct CollisionMapInfo {
	bool isCeiling = false; // 天井衝突フラグ
	bool isRanding = false; // 着地フラグ
	bool ishitWall = false; // 壁接触フラグ
	Vector3 move;           // 移動量
};

/// <summary>
/// 列挙体Cornerの宣言
/// </summary>
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上
	kNumCorner    // 要素数
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

#pragma region 移動用メンバ関数

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
	/// 角度補間
	/// </summary>
	/// <param name="RotationY">角度Y</param>
	void RotateInterpolation(float rotationY);

	/// <summary>
	/// 旋回制御メンバ関数
	/// </summary>
	void TurningControl();

	/// <summary>
	/// ジャンプ制御メンバ関数
	/// </summary>
	void JumpControl();

#pragma endregion

#pragma region 衝突判定用メンバ関数

	/// <summary>
	/// 指定した角の座標計算
	/// </summary>
	/// <param name="center"></param>
	/// <param name="Corner"></param>
	/// <returns></returns>
	Vector3 CornerPosition(const Vector3& center, Corner Corner);

	/// <summary>
	/// マップ衝突判定上方向
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionTop(CollisionMapInfo& info);

	/// <summary>
	/// マップ衝突判定下方向
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionBottom(CollisionMapInfo& info);

	/// <summary>
	/// マップ衝突判定右方向
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionRight(CollisionMapInfo& info);

	/// <summary>
	/// マップ衝突判定左方向
	/// </summary>
	/// <param name="info"></param>
	void MapCollisionLeft(CollisionMapInfo& info);

	/// <summary>
	/// マップ衝突判定
	/// </summary>
	/// <param name="info"></param>
	void MapCollision(CollisionMapInfo& info);

	/// <summary>
	/// 判定結果反映移動メンバ関数
	/// </summary>
	/// <param name="info"></param>
	void ReflectMove(const CollisionMapInfo& info);

	/// <summary>
	/// 天井衝突時メンバ関数
	/// </summary>
	/// <param name="info"></param>
	void CeilingContact(const CollisionMapInfo &info);

	/// <summary>
	/// 接地状態の処理
	/// </summary>
	/// <param name="info"></param>
	void RandingState(const CollisionMapInfo& info);

	/// <summary>
	/// 状態の切り替えメンバ関数
	/// </summary>
	/// <param name="info"></param>
	void StateSwitching(const CollisionMapInfo& info);

	/// <summary>
	/// 壁接触時処理メンバ関数
	/// </summary>
	/// <param name="info"></param>
	void WallContact(const CollisionMapInfo& info);

#pragma endregion

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突応答
	/// </summary>
	/// <param name="enemy"></param>
	void OnCollision(Enemy* enemy);

public://カプセル化とアクセッサ

	const WorldTransform& GetWorldTransform() { return worldTransform_; };

	const Vector3& GetVelocity() const { return velocity_; }
	
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	bool GetIsDead() const { return this->isDead_; }

private://メンバ変数

		// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 接地状態フラグ
	bool onGround_ = true;
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	static inline const float kAcceleration = 0.01f;

	static inline const float kAttenuation = 0.2f;

	static inline const float kLimitRunSpeed = 0.2f;

	static inline const float kTimeTurn = 0.3f;

	static inline const float kGravityAcceleration = 0.03f;

	static inline const float kLimitFallSpeed = 0.5f;

	static inline const float kJumpAcceleration = 0.65f;

	static inline const float kwidth = 1.85f;
	static inline const float kheight = 1.85f;

	static inline const float kAttenuationLanding = 0.5f;
	static inline const float kAttenuationWall = 1.0f;

	static inline const float kBlank = 0.1f;

	/// 自キャラの死亡フラグ
	bool isDead_ = false;

};
