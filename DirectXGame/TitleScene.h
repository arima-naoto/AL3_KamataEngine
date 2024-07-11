#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Bar.h"

/// <summary>
///　TitleSceneクラスの宣言
/// </summary>
class TitleScene {

public://メンバ関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

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

};
