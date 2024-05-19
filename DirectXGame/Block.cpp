#include "Block.h"
#include "cassert"

void Block::Initialize(Model* model, ViewProjection* viewProjection) 
{ 
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
}

void Block::Update() {}

void Block::Draw(const WorldTransform &worldTransform,const ViewProjection& viewProjection) 
{ 
	model_->Draw(worldTransform, viewProjection); 
}