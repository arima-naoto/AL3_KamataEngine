#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Rendering.h"
#include "GlobalVariables.h"

#include "cassert"
#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG


void Player::Initialize(Model* model, ViewProjection* viewProjection) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model; // モデル

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

	input_ = Input::GetInstance();

	InitializeParts();

	InitializeBehavior();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "body Translate", playerParts_[(int)IPlayerParts::body]->GetPosition());
	globalVariables->AddItem(groupName, "head Translate", playerParts_[(int)IPlayerParts::head]->GetPosition());
	globalVariables->AddItem(groupName, "L_Arm Translate", playerParts_[(int)IPlayerParts::left_arm]->GetPosition());
	globalVariables->AddItem(groupName, "R_Arm Translate", playerParts_[(int)IPlayerParts::right_arm]->GetPosition());

	ApplyGlobalVariables();

}

void Player::Update() 
{ 

	//ふるまい更新処理
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

	playerParts_[int(IPlayerParts::body)] = make_unique<PlayerBody>();
	playerParts_[int(IPlayerParts::body)]->Initialize(Model::CreateFromOBJ("float_Body"), viewProjection_);
	playerParts_[int(IPlayerParts::body)]->SetParent(&this->GetWorldTransform());
	playerParts_[int(IPlayerParts::head)] = make_unique<PlayerHead>();
	playerParts_[int(IPlayerParts::head)]->Initialize(Model::CreateFromOBJ("float_Head"),viewProjection_);
	playerParts_[int(IPlayerParts::head)]->SetParent(&playerParts_[static_cast<int>(IPlayerParts::body)]->GetWorldTransform());
	playerParts_[int(IPlayerParts::left_arm)] = make_unique<PlayerLeft_Arm>();
	playerParts_[int(IPlayerParts::left_arm)]->Initialize(Model::CreateFromOBJ("float_L_arm"),viewProjection_);
	playerParts_[int(IPlayerParts::left_arm)]->SetParent(&playerParts_[static_cast<int>(IPlayerParts::body)]->GetWorldTransform());
	playerParts_[int(IPlayerParts::right_arm)] = make_unique<PlayerRight_Arm>();
	playerParts_[int(IPlayerParts::right_arm)]->Initialize(Model::CreateFromOBJ("float_R_arm"),viewProjection_);
	playerParts_[int(IPlayerParts::right_arm)]->SetParent(&playerParts_[static_cast<int>(IPlayerParts::body)]->GetWorldTransform());

}

//ジョイスティックによる移動
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

			if (behavior_ == Behavior::kRoot) {
				UpdateMovement(move, speed);
			} else if (behavior_ == Behavior::kDash) {
				UpdateMovement(move, speed * 2);
			}
		
		}

		worldTransform_.rotation_.y = Calculator::LerpShortAngle(worldTransform_.rotation_.y, targetRotate_.y, destinationAngleY);
	}
}

void Player::UpdateMovement(Vector3 &move,float speed) {

	move = ~move * speed;

	Matrix4x4 rotateYMatrix = Rendering::MakeRotateYMatrix(viewProjection_->rotation_.y);

	move = Rendering::TransformNormal(move, rotateYMatrix);

	worldTransform_.translation_ += move;
	velocity_ = move;

	targetRotate_.y = std::atan2(move.x, move.z);

}

#pragma region ふるまいのメンバ関数

//通常行動初期化
void Player::BehaviorRootInitialize() {}

//通常行動更新
void Player::BehaviorRootUpdate() {

	for (auto& playerPart : playerParts_) {
		playerPart->Update();
	}

	// ジョイスティックによる自機の移動
	Player::JoyStickMove();

	XINPUT_STATE joyState;

	if (!input_->GetJoystickState(0, joyState)) {
		return;
	}

	// ダッシュボタンを押したら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 行動リクエストに、ダッシュをリクエストする
		behavior_ = Behavior::kDash;
	}
}

//ダッシュ初期化
void Player::BehaviorDashInitialize() { 
	workDash_.dashParameter_ = 0; 
	worldTransform_.rotation_.y = destinationAngleY;
}

//ダッシュ更新
void Player::BehaviorDashUpdate() {

	//各パーツの更新処理
	for (auto& playerPart : playerParts_) {
		playerPart->Update();
	}
	
	// ジョイスティックによる自機の移動
	Player::JoyStickMove();

	// ダッシュの時間<frame>
	const int32_t behaviorDashTime = 60; 

	// 基底の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		destinationAngleY = 1.0f;
		workDash_.dashParameter_ = 0;
		behavior_ = Behavior::kRoot;
	}
}

//ふるまいの初期化
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

			break;
		}
		behaviorRequest_ = std::nullopt;
	}
}

//ふるまいの更新処理
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

#pragma endregion

void Player::ApplyGlobalVariables() {

	
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	playerParts_[(int)IPlayerParts::body]->GetPosition() = globalVariables->GetVector3Value(groupName, "body Translate");
	playerParts_[(int)IPlayerParts::head]->GetPosition() = globalVariables->GetVector3Value(groupName, "head Translate");
	playerParts_[(int)IPlayerParts::left_arm]->GetPosition() = globalVariables->GetVector3Value(groupName, "L_Arm Translate");
	playerParts_[(int)IPlayerParts::right_arm]->GetPosition() = globalVariables->GetVector3Value(groupName, "R_Arm Translate");

}