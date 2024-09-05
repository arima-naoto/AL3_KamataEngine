#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

#pragma region 前方宣言したクラスのインクルード

#include "Player.h"
#include "InputHandler.h"
#include "ICommand.h"
#include "Enemy.h"
#include "DebugCamera.h"

#pragma endregion

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	model_ = Model::Create();
	viewProjection_.Initialize();

	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("kuppa.jpg");

	player_ = make_unique<Player>();
	player_->Initialize(model_, &viewProjection_, textureHandle_);

	inputHandler_ = make_unique<InputHandler>();
	GameScene::Command_Declaration();

	enemy_ = make_unique<Enemy>();
	enemy_->Initialize(model_, &viewProjection_, enemyTextureHandle_);

	debugCamera_ = make_unique<DebugCamera>(WinApp::kWindowWidth,WinApp::kWindowHeight);

#ifdef _DEBUG

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

#endif // _DEBUG
}

void GameScene::Update() 
{ 
	UpdateCommand();

	player_->Update(); 

	enemy_->Update();

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

//各コマンドに対応するキーに割り当て
void GameScene::Command_Declaration() {

	// 移動コマンド
	inputHandler_->Assign_MoveRight_Command2_PressKeyRightArrow();
	inputHandler_->Assign_MoveLeft_Command2_PressKeyLeftArrow();
	inputHandler_->Assign_MoveUp_Command2_PressKeyUpArrow();
	inputHandler_->Assign_MoveDown_Command2_PressKeyDownArrow();

	// 回転コマンド
	inputHandler_->Assign_RotateLeft_Command2_PressKeyA();
	inputHandler_->Assign_RotateRight_Command2_PressKeyD();

}

void GameScene::UpdateCommand() {

	virticalCommand_ = inputHandler_->InputVirtical();
	if (this->virticalCommand_) {
		virticalCommand_->Exec(*player_);
	}

	horizotalCommand_ = inputHandler_->InputHorizontal();
	if (this->horizotalCommand_) {
		horizotalCommand_->Exec(*player_);
	}

	rotateMentCommand_ = inputHandler_->InputRotateVirtical();
	if (this->rotateMentCommand_) {
		rotateMentCommand_->Exec(*player_);
	}

}

void GameScene::MoveDebugCamera() {

	#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		if (!isDebugCameraActive_) {
			isDebugCameraActive_ = true;
		} else if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
	}

#endif // _DEBUG

	if (isDebugCameraActive_) {

		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

}