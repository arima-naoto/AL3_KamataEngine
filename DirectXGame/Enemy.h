#pragma once
#include "WorldTransform.h"

/// クラスの前方宣言
class Model;
class ViewProjection;

/// <summary>
/// 敵クラス
/// </summary>
class Enemy {

public: // メンバ関数
	// 初期化処理
	void Initialize(Model* model, ViewProjection* viewProjection);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:

	void Move();

private: // メンバ変数
	// モデルデータ
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// 速度
	Vector3 velocity_;

};
