#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <AxisIndicator.h>

GameScene::GameScene() {}

GameScene::~GameScene() { 

	delete modelPlayer_;
	delete player_;

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

	modelPlayer_ = Model::CreateFromOBJ("player", true);
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	
	player_ = new Player();
	player_->Initialize(modelPlayer_, &viewProjection_,playerPosition);

	//3Dモデルデータの生成
	modelBlock_ = Model::CreateFromOBJ("block",true);

	block_ = new Block();
	block_->Initialize(modelBlock_, &viewProjection_);

	//blockTextureHandle_ = TextureManager::Load("cube/cube.jpg"); 

	//textureHandle_ = TextureManager::Load("uvChecker.png");

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//スカイドームモデルの生成
	modelSkyDome_ = Model::CreateFromOBJ("SkyDome", true);
	//スカイドームの生成
	skyDome_ = new SkyDome();
	skyDome_->Initialize(modelSkyDome_, &viewProjection_);

	//マップチップフィールドの生成
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(5000);

	//ブロックの生成
	GameScene::GenerateBlocks();


#pragma endregion
}

void GameScene::Update() { 
	skyDome_->Update(); 
	player_->Update();
#pragma region ワールドトランスフォームの更新処理

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) { continue; }
			worldTransformBlock->UpdateMatrix();
		}
	}
#pragma endregion

#pragma region デバッグカメラの更新処理

// デバッグビルドの時だけに呼び出す
#ifdef _DEBUG 
	//SPACEキーが押されている間
	if (input_->TriggerKey(DIK_SPACE)) 
	{
		//デバッグカメラ有効のフラグが立てられている場合
		if (isDebugCameraActive_ == true) {
			//デバッグカメラ有効のフラグをおる
			isDebugCameraActive_ = false;
		} else {//そうでなければ
			
			//カメラ有効のフラグを立てる
			isDebugCameraActive_ = true;
		}
	}
#endif

	//デバッグカメラ有効のフラグが立っている時に
	if (isDebugCameraActive_ == true) 
	{
		//デバッグカメラの更新処理を行う
		debugCamera_->Update();

		//デバッグカメラからビュー行列とプロジェクション行列をコピーする
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {

		//ビュープロジェクション行列の更新と行列
		viewProjection_.UpdateMatrix();
	}

#pragma endregion 
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
	
	//プレイヤーの描画
	player_->Draw();

	//ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {continue;}
			block_->Draw(*worldTransformBlock,debugCamera_->GetViewProjection());
		}
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
