#include "FollowCamera.h"
#include "ViewProjection.h"
#include "Rendering.h"
#include "input.h"

void FollowCamera::Initialize(ViewProjection* viewprojection) { 
	viewProjection_ = viewprojection; 

	input_ = Input::GetInstance();

}

void FollowCamera::Update() {

	//
	static float cameraLerp = 1.0f;

	if (target_) {
		// 追従座標の補間
		interTarget_ = Calculator::Lerp(interTarget_, target_->translation_,cameraLerp);
	}
	
	// 追従対象からのオフセット
	Vector3 offset = CalcOffset();
	// カメラ座標
	viewProjection_->translation_ = interTarget_ + offset;

	
	FollowCamera::JoyStickRotation();


	viewProjection_->UpdateViewMatrix();
}

void FollowCamera::Reset() {

	// 追従対象がいれば
	if (target_) {

		// 追従座標・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_->rotation_.y = target_->rotation_.y;
	

	}

	desticationAngleY = viewProjection_->rotation_.y;	

	// 追従対象からのオフセット
	Vector3 offset = CalcOffset();
	// カメラ座標
	viewProjection_->translation_ = interTarget_ + offset;
}

void FollowCamera::SetTatget(const WorldTransform* target) { 
	target_ = target; 
	FollowCamera::Reset();
}


void FollowCamera::JoyStickRotation() {

	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {

		const float kRotateSpeed = 1.f / 60.f;

		viewProjection_->rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
	}
}

Vector3 FollowCamera::CalcOffset() const {

	Vector3 offset = {0.0f, 2.0f, -10.0f};

	Matrix4x4 rotateYMatrix = Rendering::MakeRotateYMatrix(viewProjection_->rotation_.y);

	offset = Rendering::TransformNormal(offset, rotateYMatrix);

	return offset;
}