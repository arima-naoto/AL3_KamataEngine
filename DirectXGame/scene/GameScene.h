#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "SkyDome.h"
#include "MapChipField.h"
#include "Block.h"
#include "CameraController.h"
#include "Enemy.h"
#include "Rendering.h"
#include "DeathParticles.h"

/// ゲームのフェーズ
enum class Phase {
	kPlay,  // ゲームプレイ
	kDeath, // デス演出
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public Rendering{

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
	/// ブロックの生成
	/// </summary>
	void GenerateBlocks();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 全ての当たり判定を行う
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// フェーズゲームプレイの更新処理
	/// </summary>
	void UpdatekPlay();

	/// <summary>
	/// フェーズデス演出の更新処理
	/// </summary>
	void UpdateKDeath();

	/// <summary>
	/// フェーズ切り替えメンバ関数
	/// </summary>
	void ChangePhase();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	///デスフラグのgetter
	bool IsFinished() const { return this->isFinished_; }

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//3Dモデルデータ(プレイヤー)
	Model* modelPlayer_ = nullptr;
	Player* player_ = nullptr;

	//3Dモデルデータ(デス演出用パーティクル)
	Model* modelDeathParticles_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;

    //3Dモデルデータ(敵機)
	Model* modelEnemy_ = nullptr;
	Enemy* enemy_ = nullptr;
	
	//3Dモデルデータ(ブロック)
	Model* modelBlock_ = nullptr;
	Block* block_ = nullptr;

	//3Dモデルデータ(スカイドーム)
	Model* modelSkyDome_ = nullptr;
	SkyDome* skyDome_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr; 

	//マップチップ
	MapChipField* mapChipField_;

	CameraController* cameraController_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//テクスチャハンドル
	uint32_t blockTextureHandle_ = 0u;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//デバッグカメラを有効
	bool isDebugCameraActive_ = false;

	//現在フェーズ
	Phase phase_;

	//終了フラグ
	bool isFinished_ = false;

};
