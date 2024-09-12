#pragma once
#include "WorldTransform.h"
#include "Calculation.h"
#include "cassert"
#include <memory>
#include "list"

class Model;
class ViewProjection;
class Input;
class PlayerBullet;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public://メンバ関数

	~Player();
	
	/// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, uint32_t textureHandle,const Vector3& position);
	
	/// 更新
	void Update();
	
	/// 描画
	void Draw();

	/// 衝突時処理
	void OnCollision();

	void MoveLimit();

#pragma region 移動処理メンバ関数

	/// 右方向移動
	void MoveRight();

	/// 左方向移動
	void MoveLeft();

	/// 上方向移動
	void MoveUp();

	/// 下方向移動
	void MoveDown();

#pragma endregion

#pragma region 回転処理メンバ関数

	/// 右回転処理
	void RotateRight();

	/// 左回転処理
	void RotateLeft();

#pragma endregion

	void Attack();

	Vector3 GetWorldPosition();

	AABB GetAABB();

	const std::list<PlayerBullet*>& GetBullets() const;

	void SetParent(const WorldTransform* parent);

	Vector3 GetWorldTranslate();

	Vector3 GetWorldRotation();

private://メンバ変数

	//モデル
	Model* model_ = nullptr;

	//ビュープロジェクション
	ViewProjection *viewProjection_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	/// キャラクターの移動速度
	static inline const float kCharacterSpeed = 0.25f;
	static inline const float kRotSpeed = 0.0025f;

	Model* modelBullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	static inline const float kWidth_ = 2.0f;
	static inline const float kHeight_ = 2.0f;
	Vector3 velocity_ = {};
	Vector3 rotate_ = {};
	Vector3 translate_ = {};
};

