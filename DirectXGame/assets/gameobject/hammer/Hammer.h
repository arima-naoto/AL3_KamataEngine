#pragma once
#include "assets/process/collision/Collider.h"
#include "assets/process/math/Rendering.h"

//ハンマー(コライダークラスを継承)
class Hammer : public Collider,Rendering {

public:
	//コンストラクタ
	Hammer() = default;
	//デストラクタ
	~Hammer() override = default;
	//初期化
	void Initialize(Model* model, ViewProjection* viewProjection);
	//更新
	void Update();
	//描画
	void Draw();
	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;
	//中心座標を取得
	Vector3 GetCenterPosition() const override;
	//角度のGetter
	const Vector3& GetRotation() const;
	//角度のSetter
	void SetRotation(const Vector3& rotation);
	//親子関係を結ぶ
	void SetParent(const WorldTransform* parent);

private:

	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	WorldTransform worldTransform_;
	std::unique_ptr<Model> effect_;

};