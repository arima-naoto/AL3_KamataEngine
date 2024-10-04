#pragma once

#include "WorldTransform.h"
#include "PlayerParts.h"

#include <optional>

class Model;
class ViewProjection;
class Input;

/// <summary>
/// 自キャラ
/// </summary>
class Player {

public:

	enum class Behavior {
		kRoot,
		kDash
	};

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

	void SetViewProjection(const ViewProjection* viewProjection);

private:

	void InitializeParts();

	///ジョイスティックによる座標の移動
	void JoyStickMove();

	///通常行動初期化
	void BehaviorRootInitialize();

	///通常行動更新
	void BehaviorRootUpdate();

	void BehaviorDashInitialize();

	void BehaviorDashUpdate();

	void InitializeBehavior();

	void UpdateBehavior();
	
private://メンバ変数

	Model* model_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	const ViewProjection *viewProjection_ = nullptr;

	Input* input_ = nullptr;

	Vector3 velocity_ = {};
	Vector3 targetRotate_ = {};

	unique_ptr<IPlayerParts> playerParts_[IPlayerParts::partsNum] = {nullptr};

	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

};
