#pragma once
#include "WorldTransform.h"
#include "vector"

/// クラスの前方宣言
class Model;
class ViewProjection;

/// <summary>
/// 敵クラス
/// </summary>
class Enemy {

public: // メンバ関数
	// 初期化処理
	void Initialize(std::vector<Model*> models, ViewProjection* viewProjection);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	const std::vector<WorldTransform*> GetWorldTransform(){ return worldTransforms_;};

private:

	void InitializeWorldTransform();

	void Move();

private: // メンバ変数
	// モデルデータ
	std::vector<Model*> models_;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	// ワールド変換データ
	std::vector<WorldTransform*> worldTransforms_;

	// 速度
	Vector3 velocity_;

};
