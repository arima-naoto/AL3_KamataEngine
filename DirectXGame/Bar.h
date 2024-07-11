#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// タイトルバーを表示させるBarクラスを宣言
/// </summary>
class Bar {

public:

	void Initialize(Model* model, ViewProjection* viewProjection);

	void Update();

	void Draw();

private:

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform_;

};
