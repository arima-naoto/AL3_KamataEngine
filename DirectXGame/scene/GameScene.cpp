#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <AxisIndicator.h>
#include "MyStruct.h"
#include "Player.h"
#include "DebugCamera.h"
#include "SkyDome.h"
#include "MapChipField.h"
#include "Block.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"
#include "Goal.h"
#include "Bar.h"
#include "ExplainBar.h"

#define CREATE_ENEMY 3

GameScene::GameScene() {}

GameScene::~GameScene() { 

	//自キャラの解放
	delete modelPlayer_;
	delete player_;

	delete modelDeathParticles_;

	if (deathParticles_ != nullptr) {
		delete deathParticles_;
	}

	//敵キャラの解放
	delete modelEnemy_;

	for (auto* enemy : enemys_) {
		delete enemy;
	}

	delete modelGoal_;
	delete goal_;

	//ブロックの解放
	delete modelBlock_;
	delete block_;

	// スカイドームの解放
	delete modelSkyDome_;
	delete skyDome_;

	//ワールドトランスフォームの解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();

	//デバッグカメラの解放
	delete debugCamera_;

	//マップチップフィールドの解放
	delete mapChipField_;
}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
		for (uint32_t j = 0; j < numBlockHorizontal; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j,i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

#pragma region 各オブジェクトの生成と初期化

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(2, 3);

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField();
	// csvファイル読み込み
	mapChipField_->LoadMapChipCsv("Resources/block.csv");

	//自キャラの生成と初期化
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();
	player_->Initialize(modelPlayer_, &viewProjection_,playerPosition);
	player_->SetMapChipField(mapChipField_);

	//デス演出用パーティクルのモデルデータを読み込む
	modelDeathParticles_ = Model::CreateFromOBJ("DeathParticles", true);

	//敵キャラの生成と初期化

	modelEnemy_ = Model::CreateFromOBJ("Enemy", true);

	for (int32_t index = 0; index < CREATE_ENEMY; index++) {

		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(82 + (index * 5), 15 - (index * 1));
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);
		enemys_.push_back(newEnemy);
	}

	modelGoal_ = Model::CreateFromOBJ("goal", true);
	goal_ = new Goal();
	goal_->Initialize(modelGoal_, &viewProjection_, goalPosition);

	//ブロックの生成と初期化
	modelBlock_ = Model::CreateFromOBJ("block",true);
	block_ = new Block();
	block_->Initialize(modelBlock_, &viewProjection_);

	//スカイドームの生成と初期化
	modelSkyDome_ = Model::CreateFromOBJ("SkyDome", true);
	skyDome_ = new SkyDome();
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);


	modelClearBar_ = Model::CreateFromOBJ("GameClearText", true);
	clearBar_ = new Bar();
	clearBar_->Initialize(modelClearBar_, &viewProjection_);

	modelExplainBar_ = Model::CreateFromOBJ("ExplainText", true);
	explainBar_ = new ExplainBar();
	explainBar_->Initialize(modelExplainBar_, &viewProjection_);

#pragma region カメラコントローラ

	// カメラコントローラのインスタンス作成
	cameraController_ = new CameraController();
	// 初期化処理
	cameraController_->Initialize();
	// 追従対象をセット
	cameraController_->SetTarget(player_);
	// リセット(瞬間合わせ)
	cameraController_->Reset();

	cameraController_->SetMovableArea({21, 175, 0, 50});

#pragma endregion

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(5000);

	//ブロックの生成
	GameScene::GenerateBlocks();

#pragma endregion

	//ゲームプレイフェーズから開始
	phase_ = GamePhase::kFadeIn;
	
	fade_.Initialize();
	fade_.Start(Fade::Status::FadeIn, 3.0f);

#pragma endregion
}

/// 全ての当たり判定を行う
void GameScene::CheckAllCollision() {

#pragma region 自キャラと敵キャラの当たり判定

	// 判定対象の1と2の座標

	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();
	// 自キャラと敵弾の当たり判定

	for (auto* enemy : enemys_) {

		aabb2 = enemy->GetAABB();

		if (!player_->GetIsJump()) {
			if (IsCollision(aabb1, aabb2)) {

				player_->OnCollision(enemy);
			}
		} else if(player_->GetIsJump()){

			if (IsCollision(aabb1, aabb2)) {

				enemy->SetMoveTranslate(true);

			}

		}

		if (enemy->GetMoveTranslate()) {

			enemy->OnCollision(player_);
		}

	}

	// 自キャラがデス状態
	if (player_->GetIsDead()) {

		// デス演出フェーズに切り替え
		phase_ = GamePhase::kDeath;

		// 自キャラの座標を取得
		const Vector3& deathParticlesPosition = player_->GetWorldPosition();

		// 自キャラの座標にデスパーティクルを発生、初期化
		deathParticles_ = new DeathParticles();
		deathParticles_->Initialize(modelDeathParticles_, &viewProjection_, deathParticlesPosition);
	}

#pragma endregion

	AABB aabb3;

	aabb3 = goal_->GetAABB();

	if (IsCollision(aabb1, aabb3)) {

		goal_->OnCollision(player_);
	}

	if (goal_->IsGet()) {

		

		fade_.Start(Fade::Status::FadeOut, 2.0f);
		phase_ = GamePhase::kClearFadeOut;
	}

}

///フェーズフェードインの更新処理
void GameScene::UpdateFadeIn() {

	fade_.Update();

	// プレイヤーの更新処理
	player_->Update();

	// スカイドームの更新処理
	skyDome_->Update();

	goal_->Update();

	explainBar_->Update();

	// デバッグカメラが無効になっている時に
	if (isDebugCameraActive_ == false) {
		// カメラコントローラの更新処理を行う
		cameraController_->Update();

		// カメラコントローラからビュー行列とプロジェクション行列をコピーする
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	// ブロックの更新処理
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}

	if (fade_.IsFinished()) {
		phase_ = GamePhase::kPlay;
	}

}

///フェーズゲームプレイの更新処理
void GameScene::UpdatekPlay(){

#pragma region 各クラスの更新処理

	// スカイドームの更新処理
	skyDome_->Update();

	// プレイヤーの更新処理
	player_->Update();

	goal_->Update();

	// エネミーの更新処理
	for (auto* enemy : enemys_) {

		enemy->Update();

	}

	// デバッグカメラが無効になっている時に
	if (isDebugCameraActive_ == false) {
		// カメラコントローラの更新処理を行う
		cameraController_->Update();

		// カメラコントローラからビュー行列とプロジェクション行列をコピーする
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	// デバッグカメラの更新処理

#ifdef _DEBUG

	// SPACEキーが押されている間
	if (input_->TriggerKey(DIK_SPACE)) {
		// デバッグカメラ有効のフラグがおられている時
		if (isDebugCameraActive_ == false) {
			// フラグを立てる
			isDebugCameraActive_ = true;
		}
		// デバッグカメラ有効のフラグが立てられている場合
		else if (isDebugCameraActive_ == true) {
			// フラグを折る
			isDebugCameraActive_ = false;
		}
	}

#endif

	// デバッグカメラ有効のフラグが立っている時に
	if (isDebugCameraActive_ == true) {
		// デバッグカメラの更新処理を行う
		debugCamera_->Update();
		// デバッグカメラからビュー行列とプロジェクション行列をコピーする
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	// ブロックの更新処理
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}

	// 全ての当たり判定を行う
	GameScene::CheckAllCollision();

#pragma endregion

}

///フェーズデス演出の更新処理
void GameScene::UpdateKDeath() {

#pragma region 各クラスの更新処理

	// スカイドームの更新処理
	skyDome_->Update();

	goal_->Update();

	// エネミーの更新処理
	for (auto* enemy : enemys_) {

		enemy->Update();
	}

	// デス演出用パーティクルの更新処理
	if (deathParticles_ != nullptr) {
		deathParticles_->Update();
	}

	// デバッグカメラの更新処理

#ifdef _DEBUG

	// SPACEキーが押されている間
	if (input_->TriggerKey(DIK_SPACE)) {
		// デバッグカメラ有効のフラグがおられている時
		if (isDebugCameraActive_ == false) {
			// フラグを立てる
			isDebugCameraActive_ = true;
		}
		// デバッグカメラ有効のフラグが立てられている場合
		else if (isDebugCameraActive_ == true) {
			// フラグを折る
			isDebugCameraActive_ = false;
		}
	}

#endif

	// デバッグカメラ有効のフラグが立っている時に
	if (isDebugCameraActive_ == true) {
		// デバッグカメラの更新処理を行う
		debugCamera_->Update();
		// デバッグカメラからビュー行列とプロジェクション行列をコピーする
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	// ブロックの更新処理
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}

	if (deathParticles_ && deathParticles_->GetIsFinished()) {
		fade_.Start(Fade::Status::FadeOut, 2.0f);
		phase_ = GamePhase::kDeathFadeOut;
	}

#pragma endregion

}

void GameScene::UpdateKClearFadeOut() {

	fade_.Update();

	// プレイヤーの更新処理
	player_->Update();

	// エネミーの更新処理
	for (auto* enemy : enemys_) {

		enemy->Update();
	}

	// デバッグカメラが無効になっている時に
	if (isDebugCameraActive_ == false) {
		// カメラコントローラの更新処理を行う
		cameraController_->Update();

		// カメラコントローラからビュー行列とプロジェクション行列をコピーする
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	// デバッグカメラの更新処理

#ifdef _DEBUG

	// SPACEキーが押されている間
	if (input_->TriggerKey(DIK_SPACE)) {
		// デバッグカメラ有効のフラグがおられている時
		if (isDebugCameraActive_ == false) {
			// フラグを立てる
			isDebugCameraActive_ = true;
		}
		// デバッグカメラ有効のフラグが立てられている場合
		else if (isDebugCameraActive_ == true) {
			// フラグを折る
			isDebugCameraActive_ = false;
		}
	}

#endif

	// デバッグカメラ有効のフラグが立っている時に
	if (isDebugCameraActive_ == true) {
		// デバッグカメラの更新処理を行う
		debugCamera_->Update();
		// デバッグカメラからビュー行列とプロジェクション行列をコピーする
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	// ブロックの更新処理
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}

	if (fade_.IsFinished()) {
		isFinished_ = true;
	}

	clearBar_->Update();

}

///フェーズデス演出フェードアウトの処理
void GameScene::UpdatekDeathFadeOut() {

	fade_.Update();

	goal_->Update();

	// スカイドームの更新処理
	skyDome_->Update();

	// エネミーの更新処理
	for (auto* enemy : enemys_) {

		enemy->Update();
	}

	// ブロックの更新処理
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}

	if (fade_.IsFinished()) {
		isFinished_ = true;
	}
}

///フェーズの切り替え処理
void GameScene::ChangePhase() {

	switch (phase_) {

	case GamePhase::kFadeIn:

		GameScene::UpdateFadeIn();

		break;

	case GamePhase::kPlay:

		GameScene::UpdatekPlay();

		break;

	case GamePhase::kDeath:

		GameScene::UpdateKDeath();

		break;

	case GamePhase::kClearFadeOut:

		GameScene::UpdateKClearFadeOut();

		break;

	case GamePhase::kDeathFadeOut:

		GameScene::UpdatekDeathFadeOut();

		break;

	}

}

void GameScene::Update() { 

	GameScene::ChangePhase();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	switch (phase_) {
	case GamePhase::kFadeIn:

		// スカイドームの描画
		skyDome_->Draw();

		goal_->Draw();

		explainBar_->Draw();

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				block_->Draw(*worldTransformBlock);
			}
		}

		// プレイヤーの描画

		player_->Draw();
	
		// 敵キャラの描画
		for (auto* enemy : enemys_) {

			enemy->Draw();
		}

		fade_.Draw(commandList);

		break;

	case GamePhase::kPlay:

		// スカイドームの描画
		skyDome_->Draw();

		goal_->Draw();

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				block_->Draw(*worldTransformBlock);
			}
		}

		// プレイヤーの描画
		player_->Draw();

		// 敵キャラの描画
		for (auto* enemy : enemys_) {

			enemy->Draw();

		}

		break;

	case GamePhase::kDeath:

		// スカイドームの描画
		skyDome_->Draw();

		goal_->Draw();

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				block_->Draw(*worldTransformBlock);
			}
		}

		// 敵キャラの描画
		for (auto* enemy : enemys_) {

			enemy->Draw();
		}

		// デス演出用パーティクルの描画
		if (deathParticles_ != nullptr) {
			deathParticles_->Draw();
		}

		break;

	case GamePhase::kClearFadeOut:

		// スカイドームの描画
		skyDome_->Draw();

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				block_->Draw(*worldTransformBlock);
			}
		}

		// プレイヤーの描画

		player_->Draw();

		// 敵キャラの描画
		for (auto* enemy : enemys_) {

			enemy->Draw();
		}

		if (goal_->IsGet()) {
			clearBar_->Draw();
		}

		fade_.Draw(commandList);

		break;

	case GamePhase::kDeathFadeOut:

		// スカイドームの描画
		skyDome_->Draw();

		goal_->Draw();

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				block_->Draw(*worldTransformBlock);
			}
		}

		// 敵キャラの描画
		for (auto* enemy : enemys_) {

			enemy->Draw();
		}

		fade_.Draw(commandList);

		break;
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
