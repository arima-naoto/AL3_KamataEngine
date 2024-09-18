#include "WorldTransform.h"
#include "Rendering.h"

void WorldTransform::UpdateMatrix() {

	WorldTransform::matWorld_ = Rendering::MakeAffineMatrix(scale_, rotation_, translation_);

	WorldTransform::TransferMatrix();

}