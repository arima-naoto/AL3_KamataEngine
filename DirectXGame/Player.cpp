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

//コンボ定数表
const std::array < Player::ConstAttack, Player::ComboNum>
Player::kConstAttacks_ = {
	{
		//振りかぶり、攻撃前硬直,攻撃振り時間、硬直、各フェーズの移動速さ
         {0, 0, 60, 0, 0.0f, 0.0f, 0.4f},
         {15,15,15, 10,0.4f, 0.2f, 0.23f},
		 {0, 0, 20, 0, 0.0f, 0.0f, 0.15f}
    }
};

//初期化
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

//更新
void Player::Update() 
{ 
	InitializeBehavior();
	UpdateBehavior();

#ifdef _DEBUG
	DragFloat3("player.translate", &worldTransforms_[kBase]->translation_.x, 0.01f);
	DragFloat3("hammer.translate", &worldTransforms_[khammer]->translation_.x, 0.01f);
	DragFloat3("hammer.rotation", &worldTransforms_[khammer]->rotation_.x, 0.01f);
	DragInt("parameter", &workAttack_.attackParameter_, 0.01f);
	DragInt("combo", &workAttack_.comboIndex, 0.01f);
#endif // _DEBUG

}

//描画
void Player::Draw() { 

	// 3Dモデルを描画
	models_[kBody]->Draw(*worldTransforms_[kBody], *viewProjection_);           // 体
	models_[kHead]->Draw(*worldTransforms_[kHead], *viewProjection_);           // 頭
	models_[kLeft_arm]->Draw(*worldTransforms_[kLeft_arm], *viewProjection_);   // 左腕
	models_[kRight_arm]->Draw(*worldTransforms_[kRight_arm], *viewProjection_); // 右腕

	
	if (behavior_ == Behavior::kAttack) {
		models_[khammer]->Draw(*worldTransforms_[khammer], *viewProjection_);
	}
	
}

//ビュープロジェクションのsetter
void Player::SetViewProjection(const ViewProjection* viewProjection) { 
	viewProjection_ = viewProjection; 
}

#pragma region 初期化処理メンバ関数の定義

// 各ワールドトランスフォームの初期化
void Player::InitializeWorldTransform() {
	for (int i = 0; i < 6; i++) {
		worldTransforms_.resize(6);
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

	worldTransforms_[khammer]->parent_ = GetWorldTransform()[kBody];
	worldTransforms_[khammer]->translation_.y = 1.37f;
	worldTransforms_[khammer]->rotation_.x = 3;

}

// 浮遊ギミック初期化
void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }


// 通常行動初期化
void Player::BehaviorRootInitialize() {}

// 攻撃行動初期化
void Player::BehaviorAttackInitialize() {
	worldTransforms_[kBase]->translation_.y = 0;
	worldTransforms_[kLeft_arm]->rotation_.x = -1.53f;
	worldTransforms_[kRight_arm]->rotation_.x = -1.53f;
	worldTransforms_[khammer]->rotation_.x = 3;
	

	workAttack_.attackParameter_ = 0;
}

// ダッシュ初期化
void Player::BehaviorDashInitialize() {
	workDash_.dashParameter_ = 0;
	worldTransforms_[kBase]->rotation_.y = destinationAngleY;
	worldTransforms_[kLeft_arm]->rotation_.x = 0.5f;
	worldTransforms_[kRight_arm]->rotation_.x = 0.5f;
}

// ジャンプ初期化
void Player::BehaviorJumpInitialize() {

	worldTransforms_[kBody]->translation_.y = 0;
	worldTransforms_[kLeft_arm]->rotation_.x = 0;
	worldTransforms_[kRight_arm]->rotation_.x = 0;

	// ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;
	// ジャンプ初速を与える
	velocity_.y =  kJumpFirstSpeed;
}

// ふるまいの初期化
void Player::InitializeBehavior() {

	 if (behaviorRequest_) {
		// ふるまいを変更する
		behavior_ = behaviorRequest_.value();

		// ふるまい初期化をメンバ関数ポインタで呼び出す
		(this->*behaviorInitializeTable[static_cast<size_t>(behavior_)])();
	
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
	   
		velocity_ = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX ,0.f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		
		if (Calculator::Length(velocity_) > threshold) {
			isMoving = true;
		}

		if (isMoving) {

			velocity_ = ~velocity_ * speed;

			Matrix4x4 rotateYMatrix = Rendering::MakeRotateYMatrix(viewProjection_->rotation_.y);

			velocity_ = Rendering::TransformNormal(velocity_, rotateYMatrix);

			worldTransforms_[kBase]->translation_ += velocity_;
			velocity_ = velocity_;

			targetRotate_.y = std::atan2(velocity_.x, velocity_.z);
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

	//攻撃ボタンを押したら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
		//攻撃リクエスト
		behaviorRequest_ = Behavior::kAttack;
	}

	//ダッシュボタンを押したら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// ダッシュリクエスト
		behaviorRequest_ = Behavior::kDash;
	}

	//ジャンプボタンを押したら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		// ジャンプリクエスト
		behaviorRequest_ = Behavior::kJump;
	}

}

// 攻撃行動更新
void Player::BehaviorAttackUpdate() {

#pragma region コンボ継続判定

	XINPUT_STATE joyStatePre;
	XINPUT_STATE joyState;

	//コンボ条件に達していない
	if (workAttack_.comboIndex < ComboNum) {
		// 今回のゲームパッドの状態を取得 && 前回のゲームパッドの状態を取得
		if (input_->GetJoystickState(0,joyState) && input_->GetJoystickStatePrevious(0,joyStatePre)) {
			//攻撃ボタンをトリガーしたら
			if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) && !(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
				//コンボ有効
				workAttack_.comboNext = true;
				workAttack_.attackParameter_ = 0;
				workAttack_.comboIndex += 1;

				if (workAttack_.comboIndex == 1) {
					worldTransforms_[kLeft_arm]->rotation_.x = 0.0f;
					worldTransforms_[kRight_arm]->rotation_.x = 0.0f;
					worldTransforms_[khammer]->rotation_ = {3.0f, 0.0f, 0.0f};
				}
			}
		}
	}

#pragma endregion

#pragma region コンボ切り替えまたは攻撃終了

	//予備動作の時間

	int32_t anticipationTime = kConstAttacks_[workAttack_.comboIndex].anticipationTime;
	int32_t chargeTime = kConstAttacks_[workAttack_.comboIndex].chargeTime;
	int32_t swingTime = kConstAttacks_[workAttack_.comboIndex].swingTime;
	int32_t recoveryTime = kConstAttacks_[workAttack_.comboIndex].recoveryTime;

	int32_t TotalTime = anticipationTime + chargeTime + swingTime + recoveryTime;

	DragInt("anticipation", &anticipationTime, 0.01f);

	//既定の時間経過で通常行動に戻る
	if (++workAttack_.attackParameter_ >= TotalTime) {

		//コンボ継続なら次のコンボに進む
		if (workAttack_.comboNext) {
			//コンボ継続フラグをリセット
			workAttack_.comboNext = false;

			workAttack_.attackParameter_ = 0;
			workAttack_.comboIndex = 0;

			//各パーツの角度を次のコンボ用に初期化
			worldTransforms_[kLeft_arm]->rotation_.x = 0.0f;
			worldTransforms_[kRight_arm]->rotation_.x = 0.0f;
			worldTransforms_[khammer]->rotation_ = {3.0f,0.0f,0.0f};
		}
		//コンボ継続出ないなら攻撃を終了して通常行動に戻る
		else {
			worldTransforms_[khammer]->rotation_.x = 3;
			behaviorRequest_ = Behavior::kRoot;
		}
	}

#pragma endregion 



#pragma region コンボ中のパーツ制御

	//コンボ段階によってモーションに分岐
	switch (workAttack_.comboIndex) {

		//0:右から反時計回り
	case 0:

		if (workAttack_.attackParameter_ > 0 && workAttack_.attackParameter_ < 55) {
			worldTransforms_[kBase]->rotation_.y -= kConstAttacks_[0].swingTime / 9.729f;
			worldTransforms_[khammer]->rotation_.x = 1.58f;
		}

		break;

		//1:上から振り下ろし
	case 1:

		if (workAttack_.attackParameter_ > 0 && workAttack_.attackParameter_ < 10) {
			worldTransforms_[kLeft_arm]->rotation_.x -= kConstAttacks_[1].anticipationSpeed;
			worldTransforms_[kRight_arm]->rotation_.x -= kConstAttacks_[1].anticipationSpeed;
			worldTransforms_[khammer]->rotation_.x -= kConstAttacks_[1].anticipationSpeed;
		}

		if (workAttack_.attackParameter_ > 15 && workAttack_.attackParameter_ < 25) {

			Vector3 forward = Rendering::TransformNormal({0, 0, 1}, worldTransforms_[kBase]->matWorld_);
			worldTransforms_[kBase]->translation_ += forward * kConstAttacks_[1].chargeSpeed;
		}

		if (workAttack_.attackParameter_ > 30 && workAttack_.attackParameter_ < 40) {
			worldTransforms_[kLeft_arm]->rotation_.x += kConstAttacks_[1].swingSpeed;
			worldTransforms_[kRight_arm]->rotation_.x += kConstAttacks_[1].swingSpeed;
			worldTransforms_[khammer]->rotation_.x += 0.2422f;
		} 


		break;

		//2:右からホームラン
	case 2:

		worldTransforms_[kLeft_arm]->rotation_.x = -1.53f;
		worldTransforms_[kRight_arm]->rotation_.x = -1.53f;
		worldTransforms_[khammer]->rotation_ = {0.0f, -1.58f, 4.72f};
		if (workAttack_.attackParameter_ > 0 && workAttack_.attackParameter_ < 15) {
			worldTransforms_[kBase]->rotation_.y += kConstAttacks_[2].swingSpeed;
		}

		break;
	}

#pragma endregion 
};

// ダッシュ更新
void Player::BehaviorDashUpdate() {

	const float dashSpeed = 0.7f;
	JoyStickMove(dashSpeed);

	//ダッシュの時間<frame>
	const int32_t behaviorDashTime = 60;
	//既定の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}

}

// ジャンプ更新
void Player::BehaviorJumpUpdate() {

	// 移動
	worldTransforms_[kBase]->translation_ += velocity_;
	// 重力加速度
	const float kGravityAcceleration = 0.075f;
	// 加速度ベクトル
	Vector3 accelerationVector = {0, -kGravityAcceleration, 0};
	// 加速する
	velocity_ += accelerationVector;
	//着地
	if (worldTransforms_[kBase]->translation_.y <= 0.0f) {
		worldTransforms_[kBase]->translation_.y = 0;
		//ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}

}

// ふるまいの更新処理
void Player::UpdateBehavior() {

	//ふるまい更新をメンバ関数ポインタで呼び出す
	(this->*behaviorUpdateTable[static_cast<size_t>(behavior_)])();

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

void (Player::*Player::behaviorInitializeTable[])(){ 
	&Player::BehaviorRootInitialize,
	&Player::BehaviorAttackInitialize,
	&Player::BehaviorDashInitialize,
	&Player::BehaviorJumpInitialize
};

void (Player::*Player::behaviorUpdateTable[])(){ 
	&Player::BehaviorRootUpdate,
	&Player::BehaviorAttackUpdate,
	&Player::BehaviorDashUpdate,
	&Player::BehaviorJumpUpdate
};