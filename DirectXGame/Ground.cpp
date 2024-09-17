#include "Ground.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

void Ground::Initialize(Model *model,ViewProjection *viewProjection) {

	assert(model);

	model_ = model;
	
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;

}

void Ground::Update() { worldTransform_.TransferMatrix(); }

void Ground::Draw() {

	model_->Draw(worldTransform_, *viewProjection_);

}