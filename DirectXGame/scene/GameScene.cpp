#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Rendering.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete modelBlock_; 

	//ワールドトランスフォームの解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//3Dモデルデータの生成
	modelBlock_ = Model::Create();

	//textureHandle_ = TextureManager::Load("uvChecker.png");

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの生成
	//player_ = new Player();

	//自キャラの初期化
	//player_->Initialize(model_,textureHandle_,&viewProjection_);

#pragma region ワールドトランスフォームの初期化

	//要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	//要素数を変更する
	//列数を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) 
	{
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	//キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) 
	{
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {

			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}

#pragma endregion
}

void GameScene::Update() { 
	//player_->Update(); 

#pragma region ワールドトランスフォームの更新処理

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
		
				// 拡大縮小行列の作成
				Rendering::MakeScaleMatrix(worldTransformBlock->scale_);

				// X軸回転行列の作成
				Rendering::MakeRotateXMatrix(worldTransformBlock->rotation_.x);

				// Y軸回転行列の作成
				Rendering::MakeRotateYMatrix(worldTransformBlock->rotation_.y);

				// Z軸回転行列の作成
				Rendering::MakeRotateZMatrix(worldTransformBlock->rotation_.z);

				// 回転行列の作成
				Rendering::MakeRotateMatrix(worldTransformBlock->rotation_);

				// 平行移動行列の作成
				Rendering::MakeTranslateMatrix(worldTransformBlock->translation_);

				// アフィン変換行列の作成
				worldTransformBlock->matWorld_ = Rendering::MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();

			
		}
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
	//player_->Draw();

	//ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
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
