#include "PlayerParts.h"

#include <cmath>
using namespace std;

#define p_vi 3.14f

void PlayerBody::Initialize(Model* model, const ViewProjection* viewProjection) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform.Initialize();
	translate_ = {0.0f, 0.0f, 0.f};

	InitializeFloatingGimmick();
}

void PlayerBody::Update() {

	SetAffine();

	UpdateFloatingGimmick();

	ImGui::DragFloat3("body.scale", &worldTransform.scale_.x, 0.01f);
	ImGui::DragFloat3("body.rotation", &worldTransform.rotation_.x, 0.01f);
	ImGui::DragFloat3("body.translate", &worldTransform.translation_.x, 0.01f);

	worldTransform.UpdateMatrix();
}

void PlayerBody::Draw() { model_->Draw(worldTransform, *viewProjection_); }

void PlayerBody::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void PlayerBody::UpdateFloatingGimmick() {

	const uint16_t cycle = 120;

	const float step = 2.0f * p_vi / cycle;

	floatingParameter_ += step;

	floatingParameter_ = fmod(floatingParameter_, 2.0f * p_vi);

	const float amplitude = 0.2f;

	worldTransform.translation_.y = sin(floatingParameter_) * amplitude;
}

///===========================================================================================================================================

void PlayerHead::Initialize(Model* model, const ViewProjection* viewProjection) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform.Initialize();
	translate_ = {0.0f, 1.504f, 0.0f};

}

void PlayerHead::Update() {

	SetAffine();

	ImGui::DragFloat3("head.scale", &worldTransform.scale_.x, 0.01f);
	ImGui::DragFloat3("head.rotation", &worldTransform.rotation_.x, 0.01f);
	ImGui::DragFloat3("head.translate", &worldTransform.translation_.x, 0.01f);

	worldTransform.UpdateMatrix();
}

void PlayerHead::Draw() {

	model_->Draw(worldTransform, *viewProjection_);

}

///===========================================================================================================================================

void PlayerLeft_Arm::Initialize(Model* model, const ViewProjection* viewProjection) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;


	worldTransform.Initialize();
	translate_ = {-0.527f, 1.262f, 0.0f};

	InitializeFloatingGimmick();
}

void PlayerLeft_Arm::Update() {

	SetAffine();

    UpdateFloatingGimmick();

    ImGui::DragFloat3("Left_Arm.scale", &worldTransform.scale_.x, 0.01f);
	ImGui::DragFloat3("Left_Arm.rotation", &worldTransform.rotation_.x, 0.01f);
	ImGui::DragFloat3("Left_Arm.translate", &worldTransform.translation_.x, 0.01f);

	worldTransform.UpdateMatrix();
}

void PlayerLeft_Arm::Draw() { model_->Draw(worldTransform, *viewProjection_); }

void PlayerLeft_Arm::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void PlayerLeft_Arm::UpdateFloatingGimmick() {

	const uint16_t cycle = 120;

	const float step = 2.0f * p_vi / cycle;

	floatingParameter_ += step;

	floatingParameter_ = fmod(floatingParameter_, 2.0f * p_vi);

	const float amplitude = 0.5f;

	worldTransform.rotation_.x = sin(floatingParameter_) * amplitude;
}

///===========================================================================================================================================

void PlayerRight_Arm::Initialize(Model* model, const ViewProjection* viewProjection) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform.Initialize();
	translate_ = {0.527f, 1.262f, 0.0f};

	InitializeFloatingGimmick();
}

void PlayerRight_Arm::Update() {

	SetAffine();

	UpdateFloatingGimmick();

	ImGui::DragFloat3("Right_Arm.scale", &worldTransform.scale_.x, 0.01f);
	ImGui::DragFloat3("Right_Arm.rotation", &worldTransform.rotation_.x, 0.01f);
	ImGui::DragFloat3("Right_Arm.translate", &worldTransform.translation_.x, 0.01f);

	worldTransform.UpdateMatrix();
}

void PlayerRight_Arm::Draw() { model_->Draw(worldTransform,*viewProjection_); }

void PlayerRight_Arm::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void PlayerRight_Arm::UpdateFloatingGimmick() {

	const uint16_t cycle = 120;

	const float step = 2.0f * p_vi / cycle;

	floatingParameter_ += step;

	floatingParameter_ = fmod(floatingParameter_, 2.0f * p_vi);

	const float amplitude = 0.5f;

	worldTransform.rotation_.x = sin(floatingParameter_) * amplitude;
}