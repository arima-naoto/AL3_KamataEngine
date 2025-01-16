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
	kFace,         // 顔面
	kEye,          // 目

	kL_Protrution, // 左突起
	kUpperL_Arm,   // 左上腕
	kL_Forearm,    // 左前腕
	kL_Hand,       // 左手
	kL_Temo,       // 左もも
	kL_Sin,        // 左膝
	kL_Foot,       // 左脚
	
	kR_Protrution, // 右突起
	kUpperR_Arm,   // 右上腕
	kR_Forearm,    // 右前腕
	kR_Hand,       // 右手
	kR_Temo,       // 右もも
	kR_Sin,        // 右膝
	kR_Foot,       // 右脚
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
