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

	models_[kBody]->Draw(*worldTransforms_[kBody], *viewProjection_);
	models_[kFace]->Draw(*worldTransforms_[kFace], *viewProjection_);
	models_[kEye]->Draw(*worldTransforms_[kEye], *viewProjection_);

	models_[kL_Protrution]->Draw(*worldTransforms_[kL_Protrution], *viewProjection_);
	models_[kUpperL_Arm]->Draw(*worldTransforms_[kUpperL_Arm], *viewProjection_);
	models_[kL_Forearm]->Draw(*worldTransforms_[kL_Forearm], *viewProjection_);
	models_[kL_Hand]->Draw(*worldTransforms_[kL_Hand], *viewProjection_);

	models_[kL_Temo]->Draw(*worldTransforms_[kL_Temo], *viewProjection_);
	models_[kL_Sin]->Draw(*worldTransforms_[kL_Sin], *viewProjection_);
	models_[kL_Foot]->Draw(*worldTransforms_[kL_Foot], *viewProjection_);

	models_[kR_Protrution]->Draw(*worldTransforms_[kR_Protrution], *viewProjection_);
	models_[kUpperR_Arm]->Draw(*worldTransforms_[kUpperR_Arm], *viewProjection_);
	models_[kR_Forearm]->Draw(*worldTransforms_[kR_Forearm], *viewProjection_);
	models_[kR_Hand]->Draw(*worldTransforms_[kR_Hand], *viewProjection_);
	models_[kR_Temo]->Draw(*worldTransforms_[kR_Temo], *viewProjection_);
	models_[kR_Sin]->Draw(*worldTransforms_[kR_Sin], *viewProjection_);
	models_[kR_Foot]->Draw(*worldTransforms_[kR_Foot], *viewProjection_);
};

///ワールドトランスフォームの初期化処理
void Player::InitializeWorldTransform() {

	for (int i = 0; i < kPartsNum; i++) {
		worldTransforms_.resize(kPartsNum);
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
	worldTransforms_[kBody]->parent_ = GetWorldTransform()[kBase];
	worldTransforms_[kBody]->translation_.y = 1.98f;
	
	// 顔系パーツ
	worldTransforms_[kFace]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kEye]->parent_ = GetWorldTransform()[kFace];

	// 左側のパーツ
	worldTransforms_[kL_Protrution]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kUpperL_Arm]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kL_Forearm]->parent_ = GetWorldTransform()[kUpperL_Arm];
	worldTransforms_[kL_Hand]->parent_ = GetWorldTransform()[kL_Forearm];

	worldTransforms_[kL_Temo]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kL_Sin]->parent_ = GetWorldTransform()[kL_Temo];
	worldTransforms_[kL_Foot]->parent_ = GetWorldTransform()[kL_Sin];

	// 右側のパーツ(Upper)
	worldTransforms_[kR_Protrution]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kUpperR_Arm]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kR_Forearm]->parent_ = GetWorldTransform()[kUpperR_Arm];
	worldTransforms_[kR_Hand]->parent_ = GetWorldTransform()[kR_Forearm];

	worldTransforms_[kR_Temo]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kR_Sin]->parent_ = GetWorldTransform()[kR_Temo];
	worldTransforms_[kR_Foot]->parent_ = GetWorldTransform()[kR_Sin];
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
	DragFloat3("Body Translate", &worldTransforms_[kBody]->translation_.x, 0.01f);
	DragFloat3("Body Rotation", &worldTransforms_[kBody]->rotation_.x, 0.01f);
#endif // _DEBUG
}

///ビュープロジェクションのセッター
void Player::SetViewProjection(const ViewProjection* viewProjection) {
	viewProjection_ = viewProjection; 
}