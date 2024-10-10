#pragma once

#include "WorldTransform.h"
#include "vector"
#include <optional>

class Model;
class ViewProjection;
class Input;

//プレイヤーパーツの列挙体
enum Parts {
	kBase,
	kBody,
	kHead,
	kLeft_arm,
	kRight_arm
};

/// <summary>
/// 自キャラ
/// </summary>
class Player {

public:

	enum class Behavior {
		kRoot,
		kDash,
		kJump,
	};

	//ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用の媒介変数
		int32_t dashParameter_;
	};

public://メンバ関数

	/// 初期化
	void Initialize(std::vector<Model*> models, ViewProjection* viewProjection);

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

	///ふるまいのゲッター
	const Behavior& GetBehevior() { return this->behavior_; }

private:

#pragma region 初期化メンバ関数

	/// 各ワールドトランスフォーム初期化
	void InitializeWorldTransform();

	/// 浮遊ギミック初期化
	void InitializeFloatingGimmick();

	/// 通常行動初期化
	void BehaviorRootInitialize();

	/// ダッシュ初期化
	void BehaviorDashInitialize();

	///ジャンプ行動初期化
	void BehaviorJumpInitialize();

	/// ふるまい初期化
	void InitializeBehavior();

#pragma endregion

	///ジョイスティックによる座標の移動
	void JoyStickMove(const float speed);

#pragma region 更新処理メンバ関数

	/// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	///通常行動更新
	void BehaviorRootUpdate();

	///ダッシュ更新
	void BehaviorDashUpdate();	

	///ジャンプ行動初期化
	void BehaviorJumpUpdate();

	///ふるまい更新
	void UpdateBehavior();

#pragma endregion

	/// デバッグテキスト描画
	void DrawDebugText();

	/// 調整項目の適用
	void ApplyGlobalVariables();

	
private://メンバ変数

	std::vector<Model*> models_;

	//ワールド変換データ
	std::vector<WorldTransform*> worldTransforms_;

	//ビュープロジェクション
	const ViewProjection *viewProjection_ = nullptr;

	Input* input_ = nullptr;

	Vector3 velocity_ = {};
	Vector3 targetRotate_ = {};

	/// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	/// 浮遊移動のサイクル<frame>
	static inline int32_t cycle_ = 90;
	/// 浮遊の振幅
	static inline float amplitube = 0.2f;
	static inline float armAngle_ = 0.5f;

	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	WorkDash workDash_;
	float destinationAngleY = 1.0f;

};
