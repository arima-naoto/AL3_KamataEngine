#include "FollowCamera.h"
#include "ViewProjection.h"
#include "assets/process/math/Rendering.h"
#include "input.h"

#include "assets/gameobject/player/Player.h"
#include "assets/gameobject/lockOn/LockOn.h"

#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG


void FollowCamera::Initialize(ViewProjection* viewprojection) { 
	viewProjection_ = viewprojection; 

	input_ = Input::GetInstance();

}

void FollowCamera::Update() {

	//カメラ補間変数
	static float cameraLerp = 0.13f;

	if (target_) {
		// 追従座標の補間
		interTarget_ = Calculator::Lerp(interTarget_, target_->translation_,cameraLerp);
	}

	if (lockOn_ && lockOn_->ExistTarget()) {
		//ロックオン対象の座標取得
		Vector3 lockOnPos = lockOn_->GetTargetPosition();
		// 追従対象からロックオン対象へのベクトルを求める
		Vector3 sub = lockOnPos - viewProjection_->translation_;
		// Y軸周り角度
		viewProjection_->rotation_.y = std::atan2(sub.x, sub.z);
	} else {
		// ジョイスティックによるカメラの回転
		FollowCamera::JoyStickRotation();
	}
	
	// 追従対象からのオフセット
	Vector3 offset = CalcOffset();
	// カメラ座標
	viewProjection_->translation_ = interTarget_ + offset;
	
	//ビュー行列の更新
	viewProjection_->UpdateViewMatrix();
}

void FollowCamera::Reset() {

	// 追従対象がいれば
	if (target_) {
		// 追従座標・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_->rotation_.y = Rendering::LerpShortAngle(viewProjection_->rotation_.y, target_->rotation_.y, 0.025f);
	}

	desticationAngleY = viewProjection_->rotation_.y;

	// 追従対象からのオフセット
	Vector3 offset = CalcOffset();
	// カメラ座標
	viewProjection_->translation_ = interTarget_ + offset;
}

void FollowCamera::SetTarget(const WorldTransform* target) { 
	target_ = target; 
	FollowCamera::Reset();
}


void FollowCamera::JoyStickRotation() {

	XINPUT_STATE joyState;

	if (!input_->GetJoystickState(0, joyState)) {
		return;
	}

	const float kRotateSpeed = 1.f / 60.f;

	viewProjection_->rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;

	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)) {
		FollowCamera::Reset();
	}
};

Vector3 FollowCamera::CalcOffset() const {

	Vector3 offset = {0.0f, 2.0f, -10.0f};

	Matrix4x4 rotateYMatrix = Rendering::MakeRotateYMatrix(viewProjection_->rotation_.y);

	offset = Rendering::TransformNormal(offset, rotateYMatrix);

	return offset;
}