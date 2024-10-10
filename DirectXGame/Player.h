#pragma once

#include "WorldTransform.h"
#include "vector"

class Model;
class ViewProjection;
class Input;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public://メンバ関数

	/// 初期化
	void Initialize(std::vector<Model*>models, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const std::vector<WorldTransform*> GetWorldTransform() { return worldTransforms_; };

	void SetViewProjection(const ViewProjection* viewProjection);

private:

	void InitializeWorldTransform();

	///ジョイスティックによる座標の移動
	void JoyStickMove();
	
private://メンバ変数

	std::vector<Model*> models_;

	//ワールド変換データ
	std::vector<WorldTransform*>worldTransforms_;

	//ビュープロジェクション
	const ViewProjection *viewProjection_ = nullptr;

	Input* input_ = nullptr;

	Vector3 velocity_ = {};
	Vector3 targetRotate_ = {};

};
