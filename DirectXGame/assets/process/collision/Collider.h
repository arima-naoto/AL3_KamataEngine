#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "CollisionTypeIdDef.h"
#include "memory"

// 衝突判定オブジェクト
class Collider {
public:

	// 仮想デストラクタ
	virtual ~Collider() = default;
	//初期化
	void Initialize();
	//ワールドトランスフォームの初期化
	void UpdateWorldTransform();
	//描画
	void Draw(Model* model, const ViewProjection& viewProjection);
	// 衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]]Collider *other){}
	//中心座標を取得
	virtual Vector3 GetCenterPosition() const = 0;
	// 半径の取得
	float GetRadius() { return radius_; }
	// 半径の設定
	void SetRadius(float radius) { radius_ = radius; }
	//種別IDを取得
	uint32_t GetTypeID() const { return typeID_; }
	//種別IDを設定
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; }

private:
	// 衝突半径
	float radius_ = 1.5f;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//種別ID
	uint32_t typeID_ = 0u;
};