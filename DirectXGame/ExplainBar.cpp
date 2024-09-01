#include "ExplainBar.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

void ExplainBar::Initialize(Model* model, ViewProjection* viewProjection) {

	assert(model);

	model_ = model;
	
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();

}

void ExplainBar::Update() {

	adventTimer_++;

	if (adventTimer_ >= 180.0f) {

		isIndication = false;

	}

}

void ExplainBar::Draw() {

	if (isIndication != true){

		return;

	}

	model_->Draw(worldTransform_, *viewProjection_);

}