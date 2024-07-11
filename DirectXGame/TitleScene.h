#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Bar.h"
#include "Fade.h"



/// <summary>
///　TitleSceneクラスの宣言
/// </summary>
class TitleScene {

public://メンバ関数

	// シーンのフェーズ
	enum class TitlePhase { kFadeIn, kMain, kFadeOut };

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// フェード切り替え処理
	/// </summary>
	void ChangeFade();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	///デスフラグのgetter
	bool GetIsFinished() const { return this->isFinished_; }

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	///ビュープロジェクション
	ViewProjection viewProjection_;

	///タイトルモデル
	Model* titleBar_ = nullptr;
	Bar* bar_ = nullptr;

	//終了フラグ
	bool isFinished_ = false;

	///フェードクラス
	Fade* fade_ = nullptr;

	TitlePhase phase_ = TitlePhase::kFadeIn;

};
