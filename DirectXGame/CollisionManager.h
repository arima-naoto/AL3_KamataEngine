#pragma once
#include "list"
#include "Rendering.h"

class Collider;

//衝突マネージャ
class CollisionManager : public Rendering{

public:

	// リセット
	void Reset();

	//コライダー２つの衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	//全ての当たり判定チェック
	void CheckAllCollisions();

	void AddCollider(Collider* collider);

private:

	//コライダー
	std::list<Collider*> collider_;

};
