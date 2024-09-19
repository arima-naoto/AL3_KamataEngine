#include "FollowCamera.h"
#include "ViewProjection.h"
#include "Rendering.h"
#include "input.h"

void FollowCamera::Initialize(ViewProjection* viewprojection) { 
	viewProjection_ = viewprojection; 

	input_ = Input::GetInstance();

}

void FollowCamera::Update() {

	if (target_) {

		//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		Matrix4x4 rotateYMatrix = Rendering::MakeRotateYMatrix(viewProjection_->rotation_.y);

		offset = Rendering::TransformNormal(offset, rotateYMatrix);

		//座標をコピーしてオフセット分ずらす
		viewProjection_->translation_ = target_->translation_ + offset;

	}

	FollowCamera::JoyStickRotation();

	viewProjection_->UpdateMatrix();
}

void FollowCamera::JoyStickRotation() {

	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {

		const float kRotateSpeed = 1.f / 60.f;

		viewProjection_->rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;

	}

}