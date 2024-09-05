#include "Enemy.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

#ifdef _DEBUG
#include "imgui.h"
using namespace ImGui;
#endif // _DEBUG

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, uint32_t textureHandle) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0, 2, 30);

	textureHandle_ = textureHandle;

	debugText_ = DebugText::GetInstance();

}

void Enemy::Update() {

	Begin("enemy");
	DragFloat3("enemy.translate", &worldTransform_.translation_.x, 0.01f);
	End();

	debugText_->SetPos(0, 10);
	debugText_->Printf("enemy.Pos(%d)", worldTransform_.translation_.x);

	worldTransform_.translation_ -= Vector3(0, 0, 0.2f);

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}