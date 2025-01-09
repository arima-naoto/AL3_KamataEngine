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

	//顔系のパーツ
	models_[kFace]->Draw(*worldTransforms_[kFace], *viewProjection_);
	models_[kEyeBrows]->Draw(*worldTransforms_[kEyeBrows], *viewProjection_);
	models_[kHair]->Draw(*worldTransforms_[kHair], *viewProjection_);

	//左側のパーツ
	models_[kLeftArm]->Draw(*worldTransforms_[kLeftArm], *viewProjection_);
	models_[kLeftThigh]->Draw(*worldTransforms_[kLeftThigh], *viewProjection_);
	models_[kLeftLeg]->Draw(*worldTransforms_[kLeftLeg], *viewProjection_);

	// 左側のパーツ
	models_[kRightArm]->Draw(*worldTransforms_[kRightArm], *viewProjection_);
	models_[kRightThigh]->Draw(*worldTransforms_[kRightThigh], *viewProjection_);
	models_[kRightLeg]->Draw(*worldTransforms_[kRightLeg], *viewProjection_);

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
	worldTransforms_[kBody]->parent_ = this->GetWorldTransform()[kBase];
	worldTransforms_[kBody]->translation_.z = -4.0f;

	// 服
	worldTransforms_[kUpperClothes]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kLowerClothes]->parent_ = this->GetWorldTransform()[kBody];


	worldTransforms_[kFace]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kEyeBrows]->parent_ = this->GetWorldTransform()[kFace];
	worldTransforms_[kHair]->parent_ = this->GetWorldTransform()[kFace];

	worldTransforms_[kLeftArm]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kLeftThigh]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kLeftLeg]->parent_ = this->GetWorldTransform()[kLeftThigh];

	worldTransforms_[kRightArm]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kRightThigh]->parent_ = this->GetWorldTransform()[kBody];
	worldTransforms_[kRightLeg]->parent_ = this->GetWorldTransform()[kRightThigh];
	
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
	
	// 体パーツのデバッグテキスト
	DragFloat3("Body Translate", &worldTransforms_[kBody]->translation_.x, 0.01f);
	DragFloat3("UpperClothes scale", &worldTransforms_[kUpperClothes]->scale_.x, 0.01f);
	DragFloat3("UpperClothes Rotation", &worldTransforms_[kUpperClothes]->rotation_.x, 0.01f);
	DragFloat3("UpperClothes Translate", &worldTransforms_[kUpperClothes]->translation_.x, 0.01f);

	DragFloat3("Face", &worldTransforms_[kFace]->scale_.x, 0.01f);
	

#endif // _DEBUG
}

///ビュープロジェクションのセッター
void Player::SetViewProjection(const ViewProjection* viewProjection) {
	viewProjection_ = viewProjection; 
}