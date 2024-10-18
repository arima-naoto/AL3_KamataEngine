#pragma once
#include "list"
#include "Rendering.h"
#include "Model.h"
#include "memory"

class Collider;

//衝突マネージャ
class CollisionManager : public Rendering{

public:

	// 初期化
	void Initialize();

	// ワールドトランスフォームの更新
	void UpdateWorldTransform();

	// 描画
	void Draw(const ViewProjection& viewProjection);

	// リセット
	void Reset();

	//コライダー２つの衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	//全ての当たり判定チェック
	void CheckAllCollisions();

	void AddCollider(Collider* collider);

	void ApplyGlobalVariables();

private:

	//コライダー
	std::list<Collider*> collider_;
	//デバッグ表示用モデル
	std::unique_ptr<Model> colliderModel_ = nullptr;
   
	bool visible_ = false;

};
