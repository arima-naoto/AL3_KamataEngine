#pragma once

#include "WorldTransform.h"

class Model;
class ViewProjection;
class Input;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public://メンバ関数

	/// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const WorldTransform& GetWorldTransform() { return worldTransform_; };

private:

	///ジョイスティックによる座標の移動
	void JoyStickMove();
	
private://メンバ変数

	Model* model_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection *viewProjection_ = nullptr;

	Input* input_ = nullptr;
};
