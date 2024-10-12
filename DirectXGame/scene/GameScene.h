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

// 各クラスの前方宣言
class Player;        // プレイヤー
class Enemy;         // 敵
class Ground;        // 地面
class SkyDome;       // 天球
class FollowCamera;  // レールカメラ
class DebugCamera;   // デバッグカメラ

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

	void UpdateObject();

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

	unique_ptr<Model> modelFighterBody_ = nullptr;
	unique_ptr<Model> modelFighterHead_ = nullptr;
	unique_ptr<Model> modelFighterL_arm_ = nullptr;
	unique_ptr<Model> modelFighterR_arm_ = nullptr;
	unique_ptr<Model> modelhammer_ = nullptr;

	unique_ptr<Model> modelEnemyBody_ = {nullptr};
	unique_ptr<Model> modelEnemyL_spear_ = {nullptr};
	unique_ptr<Model> modelEnemyR_spear_ = {nullptr};

	unique_ptr<Model> modelGround_ = {nullptr};
	unique_ptr<Model> modelSkydome_ = {nullptr};

	// 自キャラ
	unique_ptr<Player> player_ = nullptr;
	// 敵
	unique_ptr<Enemy> enemy_ = nullptr;
	// 地面
	unique_ptr<Ground> ground_ = nullptr;
	// 天球
	unique_ptr<SkyDome> skyDome_ = nullptr;
	// レールカメラ
	unique_ptr<FollowCamera> followCamera_ = nullptr;

	// デバッグカメラ
	bool isDebugCameraActive_ = false;
	unique_ptr<DebugCamera> debugCamera_ = nullptr;
};
