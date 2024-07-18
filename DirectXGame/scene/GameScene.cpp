#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <AxisIndicator.h>
#include "MyStruct.h"

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
	delete enemy_;

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

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(35, 18);

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField();
	// csvファイル読み込み
	mapChipField_->LoadMapChipCsv("Resources/DebugBlock.csv");

	//自キャラの生成と初期化
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();
	player_->Initialize(modelPlayer_, &viewProjection_,playerPosition);
	player_->SetMapChipField(mapChipField_);

	//デス演出用パーティクルのモデルデータを読み込む
	modelDeathParticles_ = Model::CreateFromOBJ("DeathParticles", true);

	//敵キャラの生成と初期化
	modelEnemy_ = Model::CreateFromOBJ("Enemy", true);
	enemy_ = new Enemy();
	enemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

	//ブロックの生成と初期化
	modelBlock_ = Model::CreateFromOBJ("block",true);
	block_ = new Block();
	block_->Initialize(modelBlock_, &viewProjection_);

	//スカイドームの生成と初期化
	modelSkyDome_ = Model::CreateFromOBJ("skydome", true);
	skyDome_ = new SkyDome();
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);

	// カメラコントローラのインスタンス作成
	cameraController_ = new CameraController();
	// 初期化処理
	cameraController_->Initialize();
	// 追従対象をセット
	cameraController_->SetTarget(player_);
	// リセット(瞬間合わせ)
	cameraController_->Reset();

	cameraController_->SetMovableArea({21, 175, 0, 50});

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(5000);

	//ブロックの生成
	GameScene::GenerateBlocks();

#pragma endregion

	//ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

#pragma endregion
}

/// <summary>
/// 全ての当たり判定を行う
/// </summary>
void GameScene::CheckAllCollision() {

#pragma region 自キャラと敵キャラの当たり判定

	//判定対象の1と2の座標

	AABB aabb1, aabb2;

	//自キャラの座標
	aabb1 = player_->GetAABB();
	//自キャラと敵弾の当たり判定

	aabb2 = enemy_->GetAABB();

	// AABB同士の交差判定
	if (IsCollision(aabb1, aabb2)) {
		// 自キャラの衝突時コールバック関数を呼び出す
		player_->OnCollision(enemy_);

		enemy_->OnCollision(player_);
	}

	//自キャラがデス状態
	if (player_->GetIsDead()) {

		//デス演出フェーズに切り替え
		phase_ = Phase::kDeath;

		//自キャラの座標を取得
		const Vector3& deathParticlesPosition = player_->GetWorldPosition();
		
		//自キャラの座標にデスパーティクルを発生、初期化
		deathParticles_ = new DeathParticles();
		deathParticles_->Initialize(modelDeathParticles_, &viewProjection_, deathParticlesPosition);

	}

#pragma endregion

}

/// <summary>
/// ゲームプレイフェーズの更新処理
/// </summary>
void GameScene::UpdatekPlay() {

#pragma region 各クラスの更新処理

	// スカイドームの更新処理
	skyDome_->Update();

	// プレイヤーの更新処理
	player_->Update();

	// エネミーの更新処理
	enemy_->Update();

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


/// <summary>
/// フェーズデス演出の更新処理
/// </summary>
void GameScene::UpdateKDeath() {

#pragma region 各クラスの更新処理

	// スカイドームの更新処理
	skyDome_->Update();

	// エネミーの更新処理
	enemy_->Update();

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
		isFinished_ = true;
	}

#pragma endregion

}

/// <summary>
/// フェーズの切り替え処理
/// </summary>
void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:

		GameScene::UpdatekPlay();

		break;

	case Phase::kDeath:

		GameScene::UpdateKDeath();

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
	
	//スカイドームの描画
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

	// デス演出用パーティクルの描画
	if (deathParticles_ != nullptr) {
		deathParticles_->Draw();
	}

	// 敵キャラの描画
	enemy_->Draw();


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
