#pragma once
#include "WorldTransform.h"
#include "Calculation.h"
#include "Sprite.h"
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

	void DrawUI();

	/// 衝突時処理
	void OnCollision();

	

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

	

	Vector3 GetWorldPosition();

	Vector3 Get3DReticlePosition();

	AABB GetAABB();

	const std::list<PlayerBullet*>& GetBullets() const;

	void SetParent(const WorldTransform* parent);

	void SetVelocity(const Vector3& velocity);

	Vector3 GetWorldTranslate();

	Vector3 GetWorldRotation();

private:

	void MoveLimit();

	void Attack();

	void Layout_3DReticle();

	void Screen_Convert(float offsetZ);

	void JoyStickMove();

	void JoyStick3DReticleMove();

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
	static inline const float kRotSpeed = 0.025f;

	Model* modelBullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	static inline const float kWidth_ = 2.0f;
	static inline const float kHeight_ = 2.0f;
	Vector3 velocity_ = {};
	Vector3 rotate_ = {};
	Vector3 translate_ = {};

	WorldTransform worldTransform3DReticle_;
	//Model* modelReticle_ = nullptr;

	uint32_t textureReticle_ = 0u;
	Sprite* sprite2DReticle_ = nullptr;

	Vector3 posNear = {};
	Vector3 posFar = {};
	Vector2 spritePosition = {};

};

