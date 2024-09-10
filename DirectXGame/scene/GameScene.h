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
class InputHandler;
class ICommand;
class Enemy;
class DebugCamera;

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

	///コマンド宣言
	void Command_Declaration();
	
	void UpdateCommand();

	void MoveDebugCamera();

	void CheckAllCollision();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Model* model_ = nullptr;
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

	unique_ptr<Enemy> enemy_ = nullptr;
	unique_ptr<DebugCamera> debugCamera_ = nullptr;

	bool isDebugCameraActive_ = false;

};
