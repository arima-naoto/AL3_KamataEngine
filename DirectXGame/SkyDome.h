#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// SkyDomeクラス
/// </summary>
class SkyDome 
{
public:// メンバ関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model *model,ViewProjection *viewProjection);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:// メンバ変数

	// ワールド変換データ
	WorldTransform worldTransform_;
	//モデルクラス
	Model* model_ = nullptr;
	//ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
};
