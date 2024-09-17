#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "memory"
using namespace std;

class Player;
class Ground;
class DebugCamera;

enum Object {
	kPlayer,
	kGround
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	void CreateModel();
	void InitializeObject();

	void MoveDebugCamera();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// 3Dモデルデータ
	unique_ptr<Model> model_[2] = {nullptr};

	// 自キャラ
	unique_ptr<Player> player_ = nullptr;
	// 地面
	unique_ptr<Ground> ground_ = nullptr;

	//デバッグカメラ
	bool isDebugCameraActive_ = false;
	unique_ptr<DebugCamera> debugCamera_ = nullptr;
};
