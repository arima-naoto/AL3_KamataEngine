#include "TitleScene.h"
#include "Player.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {

	delete titleBar_;
	delete bar_;

	///フェードクラスの解放
	delete fade_;

}

/// 初期化処理
void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	titleBar_ = Model::CreateFromOBJ("TitleLetter", true);
	bar_ = new Bar();
	bar_->Initialize(titleBar_, &viewProjection_);

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 3.0f);

}

void TitleScene::ChangeFade() {

	

	switch (phase_) 
	{
	case TitlePhase::kFadeIn:

		if (fade_->IsFinished()) {
			phase_ = TitlePhase::kMain;
		}
		
		break;
	
	case TitlePhase::kMain:
		
		bar_->Update();

		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 2.0f);
			phase_ = TitlePhase::kFadeOut;
		}

		break;
	
	case TitlePhase::kFadeOut:

		if (fade_->IsFinished()) {
			isFinished_ = true;
		}
		
		break;
	}

}

/// 更新処理
void TitleScene::Update() {	

	/// フェードクラスの更新処理
	fade_->Update();

	///フェードの切り替え
	TitleScene::ChangeFade();

}

/// 描画処理
void TitleScene::Draw() {
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

	
	bar_->Draw();

	fade_->Draw(commandList);


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