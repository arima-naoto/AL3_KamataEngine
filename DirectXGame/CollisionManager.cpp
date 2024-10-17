#include "CollisionManager.h"
#include "Collider.h"

void CollisionManager::Reset() {
	// リストを空にする
	collider_.clear();
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	//コライダーAの中心座標を取得
	Vector3 posA = colliderA->GetCenterPosition();
	//コライダーBの中心座標を取得
	Vector3 posB = colliderB->GetCenterPosition();
	//座標の差分ベクトル
	Vector3 subtract = posB - posA;
	//距離AとBの距離を求める
	float distance = Length(subtract);

	//コライダーAとBの合計半径
	float radius = colliderA->GetRadius() + colliderB->GetRadius();

	//球と球の交差判定
	if (distance <= radius) {
		//コライダーAの衝突時コールバッグを呼び出す
		colliderA->OnCollision();
		//コライダーBの衝突時コールバッグを呼び出す
		colliderB->OnCollision();
	}
}

void CollisionManager::CheckAllCollisions() {

	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = collider_.begin();
	for (; itrA != collider_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回る
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != collider_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}

}

void CollisionManager::AddCollider(Collider* collider) { 
	collider_.push_back(collider);
}