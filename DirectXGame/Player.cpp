#include "Player.h"
#include "Input.h"
#include "Model.h"
#include "Rendering.h"
#include "ViewProjection.h"

#include "cassert"
#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG

void Player::Initialize(std::vector<Model*> models, ViewProjection* viewProjection) {

	// NULLポインタチェック

	// 引数として受け取ったデータをメンバ変数に記録する
	models_ = models; // モデル

	// ワールド変換の初期化
	InitializeWorldTransform();

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

	input_ = Input::GetInstance();
}

void Player::Update() {

	// ジョイスティックによる自機の移動
	Player::JoyStickMove();

	// 行列を更新する
	for (auto worldTransform : worldTransforms_) {
		worldTransform->UpdateMatrix();
	}
}

void Player::Draw() {
	// 3Dモデルを描画

	for (auto model : models_) {
		for (auto worldTransform : worldTransforms_) {
			model->Draw(*worldTransform, *viewProjection_);
		}
	}
}

void Player::InitializeWorldTransform() {

	// ベース
	for (int i = 0; i < 5; i++) {
		worldTransforms_.resize(5);
		WorldTransform *worldTransform=new WorldTransform();
		worldTransform->Initialize();
		worldTransforms_[i] = worldTransform;
	}
}

void Player::JoyStickMove() {

	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {

		const float threshold = 0.7f;
		const float speed = 0.3f;
		bool isMoving = false;

		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};

		if (Calculator::Length(move) > threshold) {
			isMoving = true;
		}

		if (isMoving) {

			move = ~move * speed;

			Matrix4x4 rotateYMatrix = Rendering::MakeRotateYMatrix(viewProjection_->rotation_.y);

			move = Rendering::TransformNormal(move, rotateYMatrix);

			worldTransforms_[0]->translation_ += move;
			velocity_ = move;

			targetRotate_.y = std::atan2(move.x, move.z);
		}

		worldTransforms_[0]->rotation_.y = Calculator::LerpShortAngle(worldTransforms_[0]->rotation_.y, targetRotate_.y, 1.0f);
	}
}

void Player::SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }