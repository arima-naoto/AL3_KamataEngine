#pragma once
#include "WorldTransform.h"
#include "cassert"
#include <memory>
using namespace std;

class Model;
class ViewProjection;
class Input;
class PlayerBullet;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public://メンバ関数

	
	/// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, uint32_t textureHandle);
	
	/// 更新
	void Update();
	
	/// 描画
	void Draw();

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
	static inline const float kCharacterSpeed = 0.2f;
	static inline const float kRotSpeed = 0.02f;

	Model* modelBullet_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
};

