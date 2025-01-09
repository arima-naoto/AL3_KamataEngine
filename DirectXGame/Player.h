#pragma once

#include "WorldTransform.h"
#include "vector"

class Model;
class ViewProjection;
class Input;

//プレイヤーパーツの列挙体
enum Parts {
	kBase,         // ベース
	kBody,         // 体
	kUpperClothes, // 服
	kLowerClothes, // スカート
	kFace,         // 顔面
	kEyeBrows,     // 眉毛
	kHair,         // 髪
	kLeftArm,      // 左腕
	kLeftThigh,    // 左もも
	kLeftLeg,      // 左脚
	kRightArm,     // 右腕
	kRightThigh,   // 右もも
	kRightLeg,     // 右脚
	kPartsNum      // パーツ数
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

	//親子関係を設定
	void SettingParent();

	/// 各ワールドトランスフォーム初期化
	void InitializeWorldTransform();

	/// ジョイスティックによる移動処理
	void JoyStickMove();

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
};
