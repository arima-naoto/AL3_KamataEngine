#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete sprite_,model_,debugCamera_; 
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

	//アドレス渡しでライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {

	// 現在のスプライトの座標を取得
	Vector2 position = sprite_->GetPosition();

	// 座標を{2,1}移動
	position.x += 2.0f;
	position.y += 1.0f;

	// 移動した座標をスプライトに反映させる
	sprite_->SetPosition(position);

	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		if (audio_->IsPlaying(voiceHandle_)) // 音声が再生中であれば
		{
			audio_->StopWave(voiceHandle_); // 音声を停止する
		} else                              // そうでなければ
		{
			// 音声再生する
			voiceHandle_ = audio_->PlayWave(soundDateHandle_, true);
		}
	}

#ifdef _DEBUG

	ImGui::Begin("Begin");
	// デバッグテキストの表示
	ImGui::Text("Kamata Tarou %d.%d.%d", 2024, 4, 18);

	//float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);

	//float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3,0.0f,1.0f);
	ImGui::End();

	//デモウィンドウの表示を有効化
	ImGui::ShowDemoWindow();

	//デバッグカメラの更新
	debugCamera_->Update();

#endif
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
	sprite_->Draw();

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
	
	//3Dモデルを描画する
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), modelTextureHandle_);
	
	float lineMeshWidth = 40;
	float lineMeshHeight = 40;
	float lineMeshSpanW = 2;
	float lineMeshSpanH = 2;

	for (float h = -lineMeshWidth / 2; h <= lineMeshWidth / 2; h += lineMeshSpanW) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({h, -lineMeshHeight/ 2, 0}, {h, lineMeshHeight / 2 , 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	}

	for (float w = -lineMeshHeight / 2; w <= lineMeshHeight / 2; w += lineMeshSpanH) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({-lineMeshWidth / 2, w, 0}, {lineMeshWidth / 2, w, 0}, {0.0f, 0.0f, 1.0f, 1.0f});
	}


	//ラインの描画
	//PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ラインを描画する
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
