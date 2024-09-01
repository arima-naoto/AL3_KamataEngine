#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Rendering.h"
#include "Fade.h"
#include "vector"

#pragma region 各クラスの前方宣言

class Player;
class DebugCamera;
class SkyDome;
class MapChipField;
class Block;
class CameraController;
class Enemy;
class DeathParticles;
class Goal;
class Bar;
class ExplainBar;

#pragma endregion

/// ゲームのフェーズ
enum class GamePhase {
	kFadeIn,       // フェードイン
	kPlay,         // ゲームプレイ
	kDeath,        // デス演出
	kClearFadeOut, // クリアフェードアウト
	kDeathFadeOut  // デス演出フェードアウト
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
	/// フェーズフェードインの更新処理
	/// </summary>
	void UpdateFadeIn();

	/// <summary>
	/// フェーズゲームプレイの更新処理
	/// </summary>
	void UpdatekPlay();

	/// <summary>
	/// フェーズデス演出の更新処理
	/// </summary>
	void UpdateKDeath();

	/// <summary>
	/// フェーズクリアフェードアウトの更新処理
	/// </summary>
	void UpdateKClearFadeOut();

	/// <summary>
	/// フェーズデス演出フェードアウトの更新処理
	/// </summary>
	void UpdatekDeathFadeOut();

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
	std::vector<Enemy*> enemys_;

	Model* modelGoal_ = nullptr;
	Goal* goal_ = nullptr;
	
	//3Dモデルデータ(ブロック)
	Model* modelBlock_ = nullptr;
	Block* block_ = nullptr;

	//3Dモデルデータ(スカイドーム)
	Model* modelSkyDome_ = nullptr;
	SkyDome* skyDome_ = nullptr;

	//クリアテキスト
	Model* modelClearBar_ = nullptr;
	Bar* clearBar_ = nullptr;

	Model* modelExplainBar_ = nullptr;
	ExplainBar* explainBar_ = nullptr;

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
	GamePhase phase_;

	//終了フラグ
	bool isFinished_ = false;

	Fade fade_;
};
