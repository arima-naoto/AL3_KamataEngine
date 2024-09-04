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


	void MoveTranslate();

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

};

