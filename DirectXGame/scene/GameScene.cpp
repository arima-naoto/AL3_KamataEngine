#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"


#pragma region 前方宣言したクラスのインクルード

#include "Player.h"
#include "InputHandler.h"
#include "ICommand.h"
#include "Enemy.h"
#include "SkyDome.h"
#include "RailCamera.h"
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
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("kuppa.jpg");
	TextureManager::Load("reticle.png");

	railCamera_ = make_unique<RailCamera>();
	railCamera_->Initialize(worldTransform_.matWorld_, worldTransform_.rotation_);

	Vector3 playerPosition(0.0f, 0.0f, 50.0f);
	player_ = make_unique<Player>();
	player_->Initialize(model_, &viewProjection_, textureHandle_,playerPosition);
	player_->SetParent(&railCamera_->GetWorldTransfrom());

	inputHandler_ = make_unique<InputHandler>();
	GameScene::Command_Declaration();

	enemy_ = make_unique<Enemy>();
	enemy_->Initialize(model_, &viewProjection_, enemyTextureHandle_);
	enemy_->SetPlayer(player_.get());

	modelSkydome_ = Model::CreateFromOBJ("skydome", true); 
	skyDome_ = make_unique<SkyDome>();
	skyDome_->Initialize(modelSkydome_,&viewProjection_);



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

	ObjectUpdate();

	MoveDebugCamera();

	CheckAllCollision();
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

	skyDome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//各コマンドに対応するキーに割り当て
void GameScene::Command_Declaration() {

	// 移動コマンド
	inputHandler_->Assign_MoveRight_Command2_PressKeyD();
	inputHandler_->Assign_MoveLeft_Command2_PressKeyA();
	inputHandler_->Assign_MoveUp_Command2_PressKeyW();
	inputHandler_->Assign_MoveDown_Command2_PressKeyS();

	// 回転コマンド
	inputHandler_->Assign_RotateLeft_Command2_PressKeyQ();
	inputHandler_->Assign_RotateRight_Command2_PressKeyE();

}

void GameScene::UpdateCommand() {

	player_->SetVelocity({});
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

void GameScene::ObjectUpdate(){

	railCamera_->Update();

	player_->Update();

	enemy_->Update();

	skyDome_->Update();

	

}

void GameScene::MoveDebugCamera() {

	#ifdef _DEBUG

	if (input_->TriggerKey(DIK_RETURN)) {
		if (!isDebugCameraActive_) {
			isDebugCameraActive_ = true;
		} else if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
	}

#endif // _DEBUG

	if (!isDebugCameraActive_) {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	if (isDebugCameraActive_) {

		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
}

void GameScene::PlayerCollision(AABB aabb1, AABB aabb2, const std::list<EnemyBullet*>& enemyBullets) {

	aabb1 = player_->GetAABB();

	for (auto* bullet : enemyBullets) {
		aabb2 = bullet->GetAABB();

		if (IsCollision(aabb1, aabb2)) {

			player_->OnCollision();

			bullet->OnCollision();
		}
	}

}

void GameScene::EnemyCollision(AABB aabb1, AABB aabb2, const std::list<PlayerBullet*>& playerBullets) {

	aabb2 = enemy_->GetAABB();

	for (auto* bullet : playerBullets) {
		aabb1 = bullet->GetAABB();

		if (IsCollision(aabb1, aabb2)) {

			enemy_->OnCollision();

			bullet->OnCollision();
		}
	}

}

void GameScene::BulletCollision(AABB aabb1, AABB aabb2, 
	const std::list<PlayerBullet*>& playerBullets, const std::list<EnemyBullet*>& enemyBullets) {

	for (auto* playerBullet : playerBullets) {
		for (auto* enemyBullet : enemyBullets) {

			aabb1 = playerBullet->GetAABB();
			aabb2 = enemyBullet->GetAABB();

			if (IsCollision(aabb1, aabb2)) {

				playerBullet->OnCollision();

				enemyBullet->OnCollision();
			}
		}
	}

}

void GameScene::CheckAllCollision() {

	AABB aabb1 = {}, aabb2 = {};

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	PlayerCollision(aabb1, aabb2, enemyBullets);

	EnemyCollision(aabb1, aabb2, playerBullets);

	BulletCollision(aabb1, aabb2, playerBullets, enemyBullets);

}
