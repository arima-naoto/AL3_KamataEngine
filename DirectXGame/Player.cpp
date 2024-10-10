#include "Player.h"
#include "Input.h"
#include "Model.h"
#include "Rendering.h"
#include "ViewProjection.h"

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
	InitializeFloatingGimmick();
}

///更新処理
void Player::Update() {

	DrawDebugText();
	JoyStickMove();
	UpdateFloatingGimmick();

	//行列の更新
	for (auto worldTransform : worldTransforms_) {
		worldTransform->UpdateMatrix();
	}
}

///描画処理
void Player::Draw() {

	
	// 3Dモデルを描画
	models_[kBody]->Draw(*worldTransforms_[kBody], *viewProjection_);           // 体
	models_[kHead]->Draw(*worldTransforms_[kHead], *viewProjection_);           // 頭
	models_[kLeft_arm]->Draw(*worldTransforms_[kLeft_arm], *viewProjection_);   // 左腕
	models_[kRight_arm]->Draw(*worldTransforms_[kRight_arm], *viewProjection_); // 右腕
};

///ワールドトランスフォームの初期化処理
void Player::InitializeWorldTransform() {

	for (int i = 0; i < 5; i++) {
		worldTransforms_.resize(5);
		WorldTransform *worldTransform=new WorldTransform();
		worldTransform->Initialize();
		worldTransforms_[i] = worldTransform;
	}

	//体の親子関係
	worldTransforms_[kBody]->parent_ = GetWorldTransform()[kBase];
	
	//頭の親子関係
	worldTransforms_[kHead]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kHead]->translation_ = {0.0f, 1.504f, 0.0f};//座標設定

	//左腕の親子関係
	worldTransforms_[kLeft_arm]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kLeft_arm]->translation_ = {-0.527f, 1.262f, 0.0f};//座標設定

	//右腕の親子関係
	worldTransforms_[kRight_arm]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[kRight_arm]->translation_ = {0.527f, 1.262f, 0.0f};//座標設定
	

}

///浮遊ギミック初期化
void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

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

///浮遊ギミック更新
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

///デバッグテキスト描画
void Player::DrawDebugText() {

#ifdef _DEBUG
	SliderFloat3("Body Translate", &worldTransforms_[kBody]->translation_.x, -5.0f, 5.0f);
	SliderFloat3("Head Translate", &worldTransforms_[kHead]->translation_.x, -5.0f, 5.0f);
	SliderFloat3("L_arm Rotation", &worldTransforms_[kLeft_arm]->rotation_.x, -5.0f, 5.0f);
	SliderFloat3("R_arm Rotation", &worldTransforms_[kRight_arm]->rotation_.x, -5.0f, 5.0f);
	SliderInt("FloatingCycle", &cycle_, 1, 200);
	SliderFloat("FloaingAmplitube", &amplitube, 0, 10);
	SliderFloat("IdelArmAngleMax",&armAngle_,0,3.5f);
#endif // _DEBUG
}

///ビュープロジェクションのセッター
void Player::SetViewProjection(const ViewProjection* viewProjection) {
	viewProjection_ = viewProjection; 
}