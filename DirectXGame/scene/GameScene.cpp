#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete sprite_,model_; 
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//スプライトテクスチャハンドル
	spriteTextureHandle_ = TextureManager::Load("mario.jpg");

	//モデルテクスチャハンドル
	modelTextureHandle_ = TextureManager::Load("mario.jpg");

	//スプライトの生成
	sprite_ = Sprite::Create(spriteTextureHandle_, {100, 50});

	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//サウンドデータハンドル
	soundDateHandle_ = audio_->LoadWave("mokugyo.wav");
	//音声再生
	audio_->PlayWave(soundDateHandle_);
	voiceHandle_ = audio_->PlayWave(soundDateHandle_, true);
}

void GameScene::Update() { 

	//現在のスプライトの座標を取得
	/*Vector2 position = sprite_->GetPosition();*/

	//座標を{2,1}移動
	/*position.x += 2.0f;
	position.y += 1.0f;*/

	//移動した座標をスプライトに反映させる
	/*sprite_->SetPosition(position);*/

	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		//音声停止
		audio_->StopWave(voiceHandle_);
	}

	//デバッグテキストの表示
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
	//sprite_->Draw();

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
	model_->Draw(worldTransform_, viewProjection_, modelTextureHandle_);

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
