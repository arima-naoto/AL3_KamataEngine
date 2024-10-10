#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Rendering.h"
#include "GlobalVariables.h"

#define M_PI 3.14f

#include "cassert"
#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG


void Player::Initialize(std::vector<Model*> model, ViewProjection* viewProjection) {

	// 引数として受け取ったデータをメンバ変数に記録する
	models_ = model; // モデル

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

	input_ = Input::GetInstance();

	InitializeWorldTransform();
	InitializeFloatingGimmick();
	

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "body Translate", worldTransforms_[kBody]->translation_);
	globalVariables->AddItem(groupName, "head Translate", worldTransforms_[kHead]->translation_);
	globalVariables->AddItem(groupName, "L_Arm Translate", worldTransforms_[kLeft_arm]->translation_);
	globalVariables->AddItem(groupName, "R_Arm Translate", worldTransforms_[kRight_arm]->translation_);
	globalVariables->AddItem(groupName, "FloatingCycle", cycle_);
	globalVariables->AddItem(groupName, "FloatingAmplitube", amplitube);
	globalVariables->AddItem(groupName, "idelArmAngleMax", armAngle_);

	ApplyGlobalVariables();

}

void Player::Update() 
{ 
	InitializeBehavior();
	UpdateBehavior();
}

void Player::Draw() { 

	// 3Dモデルを描画
	models_[kBody]->Draw(*worldTransforms_[kBody], *viewProjection_);           // 体
	models_[kHead]->Draw(*worldTransforms_[kHead], *viewProjection_);           // 頭
	models_[kLeft_arm]->Draw(*worldTransforms_[kLeft_arm], *viewProjection_);   // 左腕
	models_[kRight_arm]->Draw(*worldTransforms_[kRight_arm], *viewProjection_); // 右腕

}

void Player::SetViewProjection(const ViewProjection* viewProjection) { 
	viewProjection_ = viewProjection; 
}

#pragma region 初期化処理メンバ関数の定義

// 各ワールドトランスフォームの初期化
void Player::InitializeWorldTransform() {
	for (int i = 0; i < 5; i++) {
		worldTransforms_.resize(5);
		WorldTransform* worldTransform = new WorldTransform();
		worldTransform->Initialize();
		worldTransforms_[i] = worldTransform;
	}

	// 体の親子関係
	worldTransforms_[kBody]->parent_ = GetWorldTransform()[kBase];

	// 頭の親子関係
	worldTransforms_[kHead]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kHead]->translation_ = {0.0f, 1.504f, 0.0f}; // 座標設定

	// 左腕の親子関係
	worldTransforms_[kLeft_arm]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kLeft_arm]->translation_ = {-0.527f, 1.262f, 0.0f}; // 座標設定

	// 右腕の親子関係
	worldTransforms_[kRight_arm]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kRight_arm]->translation_ = {0.527f, 1.262f, 0.0f}; // 座標設定
}

// 浮遊ギミック初期化
void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

// 通常行動初期化
void Player::BehaviorRootInitialize() {}

// ダッシュ初期化
void Player::BehaviorDashInitialize() {
	workDash_.dashParameter_ = 0;
	worldTransforms_[kBase]->rotation_.y = destinationAngleY;
}

// ふるまいの初期化
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

#pragma endregion

#pragma region 更新処理メンバ関数の定義

// ジョイスティックによる移動
void Player::JoyStickMove(const float speed) {

	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {

		const float threshold = 0.7f;
	
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

			worldTransforms_[kBase]->translation_ += move;
			velocity_ = move;

			targetRotate_.y = std::atan2(move.x, move.z);
		}

		worldTransforms_[kBase]->rotation_.y = Calculator::LerpShortAngle(
			worldTransforms_[kBase]->rotation_.y, targetRotate_.y, destinationAngleY);
	}
}

// 浮遊ギミック更新
void Player::UpdateFloatingGimmick() {

	///===================================================<浮遊アニメーション>========================================================

	// 1フレーム出のパラメータ加算値
	const float step = 2.0f * M_PI / cycle_;
	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2π超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * M_PI);
	// 浮遊を座標に反映
	worldTransforms_[kBody]->translation_.y = std::sin(floatingParameter_) * amplitube;
	worldTransforms_[kLeft_arm]->rotation_.x = std::sin(floatingParameter_) * armAngle_;
	worldTransforms_[kRight_arm]->rotation_.x = std::sin(floatingParameter_) * armAngle_;
};

// 通常行動更新
void Player::BehaviorRootUpdate() {

	const float speed = 0.3f;

	JoyStickMove(speed);
	UpdateFloatingGimmick();

	XINPUT_STATE joyState;

	if (!input_->GetJoystickState(0, joyState)) {
		return;
	}

	//ダッシュボタンを押したら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// ダッシュリクエスト
		behaviorRequest_ = Behavior::kDash;
	}

}

// ダッシュ更新
void Player::BehaviorDashUpdate() {

	const float dashSpeed = 1.0f;
	JoyStickMove(dashSpeed);

	//ダッシュの時間<frame>
	const int32_t behaviorDashTime = 60;
	//既定の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}

}

// ふるまいの更新処理
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
	for (auto worldTransform : worldTransforms_) {
		worldTransform->UpdateMatrix();
	}
}

#pragma endregion

/// デバッグテキスト描画
void Player::DrawDebugText() {

#ifdef _DEBUG
	SliderFloat3("Body Translate", &worldTransforms_[kBody]->translation_.x, -5.0f, 5.0f);
	SliderFloat3("Head Translate", &worldTransforms_[kHead]->translation_.x, -5.0f, 5.0f);
	SliderFloat3("L_arm Rotation", &worldTransforms_[kLeft_arm]->rotation_.x, -5.0f, 5.0f);
	SliderFloat3("R_arm Rotation", &worldTransforms_[kRight_arm]->rotation_.x, -5.0f, 5.0f);
	SliderInt("FloatingCycle", &cycle_, 1, 200);
	SliderFloat("FloaingAmplitube", &amplitube, 0, 10);
	SliderFloat("IdelArmAngleMax", &armAngle_, 0, 3.5f);
#endif // _DEBUG
}

void Player::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	worldTransforms_[kBody]->translation_ = globalVariables->GetVector3Value(groupName, "body Translate");
	worldTransforms_[kHead]->translation_ = globalVariables->GetVector3Value(groupName, "head Translate");
	worldTransforms_[kLeft_arm]->translation_ = globalVariables->GetVector3Value(groupName, "L_Arm Translate");
	worldTransforms_[kRight_arm]->translation_ = globalVariables->GetVector3Value(groupName, "R_Arm Translate");
	cycle_ = globalVariables->GetIntValue(groupName, "FloatingCycle");
	amplitube = globalVariables->GetfloatValue(groupName, "FloatingAmplitube");
	armAngle_ = globalVariables->GetfloatValue(groupName, "idelArmAngleMax");

}