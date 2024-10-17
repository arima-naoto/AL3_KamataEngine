#pragma once
#include "BaseCharacter.h"
#include "vector"
#include "Rendering.h"

/// <summary>
/// 敵クラス
/// </summary>
class Enemy : public BaseCharacter ,Rendering{

public: // メンバ関数
	// 初期化処理
	void Initialize(std::vector<Model*> models, ViewProjection* viewProjection) override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	Vector3 GetCenterPosition() const override;

private:

	void InitializeWorldTransform();

	void InitializeFloatingGimmick();

	void Move();

	void UpdateFloatingGimmick();

private: // メンバ変数

	// 速度
	Vector3 velocity_;

	float floatingParameter_;

};
