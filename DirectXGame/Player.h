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

	void GamePadController();

	const WorldTransform& GetWorldTransform() { return worldTransform_; };

	void SetViewProjection(const ViewProjection* viewProjection);

private:

	///ジョイスティックによる座標の移動
	void JoyStickMove();
	
private://メンバ変数

	Model* model_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	const ViewProjection *viewProjection_ = nullptr;

	Input* input_ = nullptr;

	const float speed = 0.3f;
	Vector3 move = {};
	bool isMoving = false;

	Vector3 targetRotate_ = {};
	float destinationAngleY = 0.1f;

};
