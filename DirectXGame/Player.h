#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player 
{
public://メンバ関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="texuteHandle">テクスチャハンドル</param>
	void Initialize(Model *model,uint32_t texuteHandle);

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

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
