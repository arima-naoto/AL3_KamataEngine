#pragma once
#include "WorldTransform.h"
#include "cassert"

class Model;
class ViewProjection;
class Input;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, ViewProjection* viewProjection, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 右方向移動
	/// </summary>
	void MoveRight();

	/// <summary>
	/// 左方向移動
	/// </summary>
	void MoveLeft();

	/// <summary>
	/// 上方向移動
	/// </summary>
	void MoveUp();

	/// <summary>
	/// 下方向移動
	/// </summary>
	void MoveDown();

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

};

