#pragma once
#include "WorldTransform.h"

class Model;
class ViewProjection;

/// <summary>
/// 説明テキスト用クラス
/// </summary>
class ExplainBar {

public:
	
	void Initialize(Model* model, ViewProjection* viewprojection);

	void Update();

	void Draw();

private:

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

	///説明テキストを表示させておく時間
	float adventTimer_ = 0.0f;

	///説明テキストが表示されてるか
	bool isIndication = true;


};
