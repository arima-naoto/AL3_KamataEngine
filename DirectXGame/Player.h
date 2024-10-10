#pragma once

#include "WorldTransform.h"
#include "vector"

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
public: // メンバ関数
	/// コンストラクタ
	Player() = default;

	/// デストラクタ
	~Player() = default;

	/// 初期化処理
	void Initialize(std::vector<Model*> models, ViewProjection* viewProjection);

	/// 更新
	void Update();

	/// 描画
	void Draw();

	/// 可変長配列ワールド変換データのgetter
	const std::vector<WorldTransform*> GetWorldTransform() { return worldTransforms_; };

	/// ビュープロジェクションのsetter
	void SetViewProjection(const ViewProjection* viewProjection);

private: // メンバ関数(このクラスでしか使わない関数)
	/// 各ワールドトランスフォーム初期化
	void InitializeWorldTransform();

	/// 浮遊ギミック初期化
	void InitializeFloatingGimmick();

	/// ジョイスティックによる移動処理
	void JoyStickMove();

	/// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	/// アームアニメーション更新
	void UpdateArmFloatingMovement(float& floatingValue, float& floatingParameter, int32_t cycle, float armAngle);

	/// デバッグテキスト描画
	void DrawDebugText();

private: // メンバ変数
	std::vector<Model*> models_;

	// 可変長配列ワールド変換データ
	std::vector<WorldTransform*> worldTransforms_;

	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

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
};
