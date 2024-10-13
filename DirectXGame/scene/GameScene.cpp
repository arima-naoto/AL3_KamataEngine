#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

#include "Player.h"
#include "Enemy.h"
#include "Ground.h"
#include "SkyDome.h"
#include "FollowCamera.h"
#include "DebugCamera.h"

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
	debugCamera_->SetFarZ(10000);

	//軸方向表示
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() { UpdateObject(); }

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

	player_->Draw();  // プレイヤー
	enemy_->Draw();   // 敵
	ground_->Draw();  // 地面
	skyDome_->Draw(); // 天球

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
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));

	// 敵モデルデータ
	modelEnemyBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	modelEnemyL_spear_.reset(Model::CreateFromOBJ("spear", true));
	modelEnemyR_spear_.reset(Model::CreateFromOBJ("spear", true));

	// 地面モデルデータ
	modelGround_.reset(Model::CreateFromOBJ("ground", true));

	// 天球モデルデータ
	modelSkydome_.reset(Model::CreateFromOBJ("SkyDome", true));
}

void GameScene::InitializeObject() {

	// パーツ
	std::vector<Model*> playerParts = {
	    nullptr,                  // ベース(存在していないのでnullptrにしている)
	    modelFighterBody_.get(),  // 体
	    modelFighterHead_.get(),  // 頭
	    modelFighterL_arm_.get(), // 左腕
	    modelFighterR_arm_.get(), // 右腕
	};

	// 自キャラの生成
	player_ = make_unique<Player>();
	player_->Initialize(playerParts, &viewProjection_);

	std::vector<Model*> enemyParts = {
		nullptr, 
		modelEnemyBody_.get(), 
		modelEnemyL_spear_.get(), 
		modelEnemyR_spear_.get()
	};

	enemy_ = make_unique<Enemy>();
	enemy_->Initialize(enemyParts, &viewProjection_);

	followCamera_ = make_unique<FollowCamera>();
	followCamera_->Initialize(&viewProjection_);
	followCamera_->SetTarget(player_->GetWorldTransform()[0]);

	player_->SetViewProjection(followCamera_->GetViewProjection());

	// 地面の生成
	ground_ = make_unique<Ground>();
	ground_->Initialize(modelGround_.get(), &viewProjection_);

	// 天球の生成
	skyDome_ = make_unique<SkyDome>();
	skyDome_->Initialize(modelSkydome_.get(), &viewProjection_);
}

///各オブジェクトの更新処理
void GameScene::UpdateObject() {
	player_->Update();       // プレイヤー
	enemy_->Update();        // 敵
	ground_->Update();       // 地面
	skyDome_->Update();      // 天球
	followCamera_->Update(); // レールカメラ
	MoveDebugCamera();       // デバッグカメラ
};

void GameScene::MoveDebugCamera() {

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		if (!isDebugCameraActive_) {
			isDebugCameraActive_ = true;
		} else if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
	}

#endif //_DEBUG

	if (!isDebugCameraActive_) {
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection()->matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection()->matProjection;
		viewProjection_.TransferMatrix();
	} else {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
}