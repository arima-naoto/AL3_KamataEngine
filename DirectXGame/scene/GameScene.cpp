#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


#include "Player.h"
#include "Ground.h"
#include "DebugCamera.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();


	CreateModel();
	InitializeObject();
	

	//デバッグカメラ
	debugCamera_ = make_unique<DebugCamera>(WinApp::kWindowWidth,WinApp::kWindowHeight);

	//軸方向表示
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() { 
	player_->Update(); 

	MoveDebugCamera();
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
	player_->Draw();

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

void GameScene::CreateModel() {

	// 自キャラモデルデータ
	model_[Object::kPlayer].reset(Model::CreateFromOBJ("cube", true));
	// 地面モデルデータ
	model_[Object::kGround].reset(Model::CreateFromOBJ("ground", true));

}

void GameScene::InitializeObject() {

	// 自キャラの生成
	player_ = make_unique<Player>();
	player_->Initialize(model_[Object::kPlayer].get(), &viewProjection_);
	// 地面の生成
	ground_ = make_unique<Ground>();
	ground_->Initialize(model_[Object::kGround].get(), &viewProjection_);

}

void GameScene::MoveDebugCamera() {

	if (input_->TriggerKey(DIK_RETURN)) {
		if (!isDebugCameraActive_) {
			isDebugCameraActive_ = true;
		} else if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
	}

#ifdef _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

#endif //_DEBUG
}