#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Rendering.h"



#include "cassert"
#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG


void Player::Initialize(Model* model, ViewProjection* viewProjection) {

	//NULLポインタチェック
	assert(model);
	
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model; // モデル

	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	
	input_ = Input::GetInstance();

	InitializeParts();

	BehaviorRootInitialize();

}

void Player::Update() 
{ 
	InitializeBehavior();
	UpdateBehavior();
}

void Player::Draw() { 

	for (auto &playerPart : playerParts_) {
		playerPart->Draw();
	}
}

void Player::SetViewProjection(const ViewProjection* viewProjection) { 
	viewProjection_ = viewProjection; 
}

void Player::InitializeParts() {

	playerParts_[static_cast<int>(IPlayerParts::body)] = make_unique<PlayerBody>();
	playerParts_[static_cast<int>(IPlayerParts::body)]->Initialize(Model::CreateFromOBJ("float_Body"), viewProjection_);
	playerParts_[static_cast<int>(IPlayerParts::body)]->SetParent(&this->GetWorldTransform());

	playerParts_[static_cast<int>(IPlayerParts::head)] = make_unique<PlayerHead>();
	playerParts_[static_cast<int>(IPlayerParts::head)]->Initialize(Model::CreateFromOBJ("float_Head"),viewProjection_);
	playerParts_[static_cast<int>(IPlayerParts::head)]->SetParent(&playerParts_[static_cast<int>(IPlayerParts::body)]->GetWorldTransform());

	playerParts_[static_cast<int>(IPlayerParts::left_arm)] = make_unique<PlayerLeft_Arm>();
	playerParts_[static_cast<int>(IPlayerParts::left_arm)]->Initialize(Model::CreateFromOBJ("float_L_arm"),viewProjection_);
	playerParts_[static_cast<int>(IPlayerParts::left_arm)]->SetParent(&playerParts_[static_cast<int>(IPlayerParts::body)]->GetWorldTransform());

	playerParts_[static_cast<int>(IPlayerParts::right_arm)] = make_unique<PlayerRight_Arm>();
	playerParts_[static_cast<int>(IPlayerParts::right_arm)]->Initialize(Model::CreateFromOBJ("float_R_arm"),viewProjection_);
	playerParts_[static_cast<int>(IPlayerParts::right_arm)]->SetParent(&playerParts_[static_cast<int>(IPlayerParts::body)]->GetWorldTransform());

}

void Player::JoyStickMove() {

	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {

		const float threshold = 0.7f;
		const float speed = 0.3f;
		bool isMoving = false;
	   
		Vector3 move = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX ,0.f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		
		if (Calculator::Length(move) > threshold) {
			isMoving = true;
		}

		if (isMoving) {

			move = ~move * speed;

			Matrix4x4 rotateYMatrix = Rendering::MakeRotateYMatrix(viewProjection_->rotation_.y);

			move = Rendering::TransformNormal(move, rotateYMatrix);

			worldTransform_.translation_ += move;
			velocity_ = move;

			targetRotate_.y = std::atan2(move.x, move.z);
		}

		worldTransform_.rotation_.y = Calculator::LerpShortAngle(worldTransform_.rotation_.y, targetRotate_.y, 1.0f);
	}
}

//通常行動
void Player::BehaviorRootInitialize() { InitializeParts(); }
void Player::BehaviorRootUpdate() {

	for (auto& playerPart : playerParts_) {
		playerPart->Update();
	}

	// ジョイスティックによる自機の移動
	Player::JoyStickMove();

}

//ダッシュ
void Player::BehaviorDashInitialize() {}
void Player::BehaviorDashUpdate() {}

void Player::InitializeBehavior() {

	if (behaviorRequest_) {
		// ふるまいを変更する
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Player::Behavior::kRoot:

			BehaviorRootInitialize();

			break;
		case Player::Behavior::kDash:

			BehaviorDashInitialize();
			BehaviorDashUpdate();

			break;
		}
		behaviorRequest_ = std::nullopt;
	}

}

void Player::UpdateBehavior() {

	switch (behavior_) {
	case Player::Behavior::kRoot:

		BehaviorRootUpdate();

		break;
	case Player::Behavior::kDash:

		BehaviorDashUpdate();

		break;
	}

	// 行列を更新する
	worldTransform_.UpdateMatrix();


}