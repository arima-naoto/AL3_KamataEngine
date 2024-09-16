#include "SkyDome.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

void SkyDome::Initialize(Model *model,ViewProjection *viewProjection) {

	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();

}

void SkyDome::Update() { 

	worldTransform_.rotation_.y += 0.001f;

	worldTransform_.UpdateMatrix();
}

void SkyDome::Draw() {

	model_->Draw(worldTransform_, *viewProjection_);

}