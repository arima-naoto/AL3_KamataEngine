#include "FollowCamera.h"
#include "ViewProjection.h"

void FollowCamera::Initialize(ViewProjection* viewProjection) {
	viewProjection_ = viewProjection;
}

void FollowCamera::Update() {}