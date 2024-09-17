#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Calculation.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"

#include <sstream>

#include "memory"
using namespace std;

class Player;
class InputHandler;
class ICommand;
class Enemy;
class EnemyBullet;
class SkyDome;
class RailCamera;
class DebugCamera;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public Calculation{

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

	void AddEnemyBullet(EnemyBullet* enemyBullet);

private:

	void LoadEnemyPopDate();

	void UpdateEnemyPopCommand();

	///コマンド宣言
	void Command_Declaration();
	
	void UpdateCommand();

	///各オブジェクトの更新処理
	void ObjectUpdate();

	void ObjectDraw();

	void MoveDebugCamera();

	void PlayerCollision(AABB aabb1, AABB aabb2,const std::list<EnemyBullet*>& enemyBullets);

	void EnemyCollision(AABB aabb1, AABB aabb2, const std::list<PlayerBullet*>& playerBullets);
	
	void BulletCollision(AABB aabb1, AABB aabb2, const std::list<PlayerBullet*>& playerBullets, const std::list<EnemyBullet*>& enemyBullets);

	// 全ての衝突判定
	void CheckAllCollision();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	ViewProjection viewProjection_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t enemyTextureHandle_ = 0u;

	unique_ptr<Player> player_ = nullptr;
	unique_ptr<InputHandler> inputHandler_ = nullptr;

	ICommand* virticalCommand_ = nullptr;
	ICommand* horizotalCommand_ = nullptr;
	ICommand* rotateMentCommand_ = nullptr;


	std::stringstream enemyPopCommands;

	
	std::list<Enemy*> enemies_;
	std::list<EnemyBullet*> enemyBullets_; 

	unique_ptr<SkyDome> skyDome_ = nullptr;
	unique_ptr<RailCamera> railCamera_ = nullptr;
	unique_ptr<DebugCamera> debugCamera_ = nullptr;

	bool isDebugCameraActive_ = false;
	WorldTransform worldTransform_;

	bool isWait = false;
	int waitTime_ = 120;

};
