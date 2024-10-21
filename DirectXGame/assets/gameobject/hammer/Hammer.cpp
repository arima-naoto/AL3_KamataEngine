#include "assets/gameobject/hammer/Hammer.h"
#include "assets/gameobject/enemy/Enemy.h"
#include "cassert"
#ifdef _DEBUG
#include "imgui.h"
using namespace ImGui;
#endif // _DEBUG

void Hammer::Initialize(Model* model) {

	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_.y = 1.37f;
	worldTransform_.rotation_.x = 3;

	effect_.reset(Model::CreateFromOBJ("sphere", true));
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));
}

void Hammer::Update() { 

#ifdef _DEBUG
	Begin("hammer");
	DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
	End();
#endif // DEBUG


	worldTransform_.UpdateMatrix(); 
}

void Hammer::Draw(const ViewProjection &viewProjection) { 
	model_->Draw(worldTransform_, viewProjection); 
}

void Hammer::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::KEnemy)) {
		Enemy* enemy = static_cast<Enemy*>(other);
		
	}
}

Vector3 Hammer::GetCenterPosition() const {

	// ローカル座標でのオフセット
	const Vector3 offset = {0.f, 1.5f, 0.f};
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;

}

const Vector3& Hammer::GetRotation() const { 
	return worldTransform_.rotation_; 
}

void Hammer::SetRotation(const Vector3& rotation) {
	worldTransform_.rotation_ = rotation;
}

void Hammer::SetParent(const WorldTransform* parent) {
	worldTransform_.parent_ = parent;
}