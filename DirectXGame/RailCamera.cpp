#include "RailCamera.h"
#include "Rendering.h"
#ifdef _DEBUG
#include "imgui.h"
using namespace ImGui;
#endif // _DEBUG

void RailCamera::Initialize(const Matrix4x4& matWorld, const Vector3& rotate) {
	worldTransform_.Initialize();
	worldTransform_.matWorld_ = matWorld;
	worldTransform_.rotation_ = rotate;
	worldTransform_.translation_ = Vector3({0, 0, -100});

	viewProjection_.Initialize();

}
void RailCamera::Update() {
	viewProjection_.matView = Rendering::Inverse(worldTransform_.matWorld_);
	worldTransform_.translation_ += Vector3({0,0,0.01f});
	worldTransform_.rotation_ += Vector3({0, 0.05f, 0});
	worldTransform_.UpdateMatrix();


	DragFloat3("RailCamera.rotation", &worldTransform_.rotation_.x, 0.01f);
	DragFloat3("RailCamera.translate", &worldTransform_.translation_.x, 0.01f);
}

const WorldTransform& RailCamera::GetWorldTransfrom() const { return this->worldTransform_; }

const ViewProjection& RailCamera::GetViewProjection() const { return this->viewProjection_; }