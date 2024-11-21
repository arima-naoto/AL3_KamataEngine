#include "Player.h"
#include "Input.h"
#include "Model.h"
#include "Rendering.h"
#include "ViewProjection.h"
#include "numbers"

#define M_PI 3.14f

#include "cassert"
#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG

///初期化処理
void Player::Initialize(std::vector<Model*> models, ViewProjection* viewProjection) {

	// 引数として受け取ったデータをメンバ変数に記録する
	models_ = models; // モデル

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

	input_ = Input::GetInstance();

	InitializeWorldTransform();
}

///更新処理
void Player::Update() {

	DrawDebugText();
	JoyStickMove();

	//行列の更新
	for (auto worldTransform : worldTransforms_) {
		worldTransform->UpdateMatrix();
	}
}

///描画処理
void Player::Draw() {
	// 3Dモデルを描画

	//体
	models_[kBody]->Draw(*worldTransforms_[kBody], *viewProjection_);
	models_[kUpperClothes]->Draw(*worldTransforms_[kUpperClothes], *viewProjection_);
	models_[kLowerClothes]->Draw(*worldTransforms_[kLowerClothes], *viewProjection_);
	
	//頭
	models_[kFace]->Draw(*worldTransforms_[kFace], *viewProjection_);
	models_[kEyebrow]->Draw(*worldTransforms_[kEyebrow], *viewProjection_);
	models_[kHairLeft]->Draw(*worldTransforms_[kHairLeft], *viewProjection_);
	models_[kHairRight]->Draw(*worldTransforms_[kHairRight], *viewProjection_);
	
	//左側パーツ
	models_[kLeft_arm]->Draw(*worldTransforms_[kLeft_arm], *viewProjection_);
	models_[kLeft_thigh]->Draw(*worldTransforms_[kLeft_thigh], *viewProjection_);
	models_[kLeft_leg]->Draw(*worldTransforms_[kLeft_leg], *viewProjection_);

	// 左側パーツ
	models_[kRight_arm]->Draw(*worldTransforms_[kRight_arm], *viewProjection_);
	models_[kRight_thigh]->Draw(*worldTransforms_[kRight_thigh], *viewProjection_);
	models_[kRight_leg]->Draw(*worldTransforms_[kRight_leg], *viewProjection_);
};

///ワールドトランスフォームの初期化処理
void Player::InitializeWorldTransform() {

	for (int i = 0; i < 14; i++) {
		worldTransforms_.resize(14);
		WorldTransform* worldTransform = new WorldTransform();
		worldTransform->Initialize();
		worldTransforms_[i] = worldTransform;
	}

	//親子関係を設定
	this->SettingParent();

}

void Player::SettingParent() {

	worldTransforms_[kBase]->translation_.y = -0.5f;
	//worldTransforms_[kBase]->rotation_.y = std::numbers::pi_v<float>;

	// 体
	worldTransforms_[kBody]->parent_ = this->GetWorldTransform()[kBase];
	worldTransforms_[kBody]->translation_.z = -4.0f;

	// 服
	worldTransforms_[kUpperClothes]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kLowerClothes]->parent_ = this->GetWorldTransform()[kBody];

	// 顔
	worldTransforms_[kFace]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kEyebrow]->parent_ = this->GetWorldTransform()[kFace];
	worldTransforms_[kEyebrow]->translation_.z = -0.006f;
	worldTransforms_[kHairLeft]->parent_ = this->GetWorldTransform()[kFace];
	worldTransforms_[kHairRight]->parent_ = this->GetWorldTransform()[kFace];

	// 左側
	worldTransforms_[kLeft_arm]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kLeft_arm]->translation_ = {-0.05f, 0.05f, 0.0f};


	worldTransforms_[kLeft_thigh]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kLeft_thigh]->translation_.y = 0.015f;
	worldTransforms_[kLeft_leg]->parent_ = this->GetWorldTransform()[kLeft_thigh];
	worldTransforms_[kLeft_leg]->translation_.y = 0.035f;

	// 右側
	worldTransforms_[kRight_arm]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kRight_arm]->translation_ = {0.05f, 0.05f, 0.0f};

	worldTransforms_[kRight_thigh]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kRight_thigh]->translation_.y = 0.02f;
	worldTransforms_[kRight_leg]->parent_ = this->GetWorldTransform()[kRight_thigh];
	worldTransforms_[kRight_leg]->translation_.y = 0.03f;
};

///ジョイスティックによる移動処理
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

			worldTransforms_[kBase]->translation_ += move;
			velocity_ = move;

			targetRotate_.y = std::atan2(move.x, move.z);
		}

		worldTransforms_[kBase]->rotation_.y = Calculator::LerpShortAngle(worldTransforms_[kBase]->rotation_.y, targetRotate_.y, 1.0f);
	}
}

///デバッグテキスト描画
void Player::DrawDebugText() {

#ifdef _DEBUG
	DragFloat3("Base Translate", &worldTransforms_[kBase]->translation_.x, 0.01f);
	DragFloat3("Base Rotation", &worldTransforms_[kBase]->rotation_.x, 0.01f);
	
	DragFloat3("Eyebrow Translate", &worldTransforms_[kEyebrow]->translation_.x, 0.01f);

	DragFloat3("Body Translate", &worldTransforms_[kBody]->translation_.x, 0.01f);
	DragFloat3("UpperClothes scale", &worldTransforms_[kUpperClothes]->scale_.x, 0.01f);
	DragFloat3("UpperClothes Rotation", &worldTransforms_[kUpperClothes]->rotation_.x, 0.01f);
	DragFloat3("UpperClothes Translate", &worldTransforms_[kUpperClothes]->translation_.x, 0.01f);
	
	DragFloat3("L_Arm Translate", &worldTransforms_[kLeft_arm]->translation_.x, 0.01f);
	DragFloat3("L_thigh Translate", &worldTransforms_[kLeft_thigh]->translation_.x, 0.01f);
	DragFloat3("L_leg Translate", &worldTransforms_[kLeft_leg]->translation_.x, 0.01f);

	DragFloat3("R_Arm Translate", &worldTransforms_[kRight_arm]->translation_.x, 0.01f);
	DragFloat3("R_thigh Translate", &worldTransforms_[kRight_thigh]->translation_.x, 0.01f);
	DragFloat3("R_leg Translate", &worldTransforms_[kRight_leg]->translation_.x, 0.01f);
#endif // _DEBUG
}

///ビュープロジェクションのセッター
void Player::SetViewProjection(const ViewProjection* viewProjection) {
	viewProjection_ = viewProjection; 
}