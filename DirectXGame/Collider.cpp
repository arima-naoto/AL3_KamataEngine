#include "Collider.h"

void Collider::Initialize() {
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

void Collider::UpdateWorldTransform() {
	// ワールド座標をワールドトランスフォームに適応
	worldTransform_.translation_ = GetCenterPosition();
	// ワールド行列を再計算して定数バッファに書き込む
	worldTransform_.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {
	//モデルの描画
	model->Draw(worldTransform_, viewProjection);
}