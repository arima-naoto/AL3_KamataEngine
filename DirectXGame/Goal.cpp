#include "Goal.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

void Goal::Initialize(Model *model,ViewProjection *viewProjection,const Vector3 &position) {

	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

}

void Goal::Update() {

	worldTransform_.UpdateMatrix();

}

void Goal::Draw() {

	model_->Draw(worldTransform_, *viewProjection_);

}

