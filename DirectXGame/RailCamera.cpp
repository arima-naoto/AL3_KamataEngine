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
	viewProjection_.farZ = 2000;
	viewProjection_.Initialize();

}
void RailCamera::Update() {
	viewProjection_.matView = Rendering::Inverse(worldTransform_.matWorld_);
	worldTransform_.translation_ += Vector3({0,0,-0.1f});
	worldTransform_.UpdateMatrix();
}

const WorldTransform& RailCamera::GetWorldTransfrom() const { return this->worldTransform_; }

const ViewProjection& RailCamera::GetViewProjection() const { return this->viewProjection_; }

void RailCamera::SetTranslation(Vector3 translation) { worldTransform_.translation_ = translation; }

void RailCamera::SetRotation(Vector3 rotation) { worldTransform_.rotation_ = rotation; }