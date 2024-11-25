#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "assets/process/global/GlobalVariables.h"
#include "assets/gameobject/lockOn/LockOn.h"

#define M_PI 3.14f

#include "cassert"
#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG

const std::array < Player::ConstAttack, Player::ComboNum>
Player::kConstAttacks_ = {
	{
		//振りかぶり、攻撃前硬直,攻撃振り時間、硬直、各フェーズの移動速さ
         {0, 0, 45, 0, 0.0f, 0.0f, 0.15f},
         {15,15,15, 10,0.25f, 0.2f, 0.2525f},
		 {0, 0, 20, 0, 0.0f, 0.0f, 0.15f}
    }
};

// 初期化処理
void Player::Initialize(std::vector<Model*> models, ViewProjection* viewProjection) {

	BaseCharacter::Initialize(models, viewProjection);
	viewProjection_ = viewProjection;
	input_ = Input::GetInstance();

	InitializeWorldTransform();
	InitializeFloatingGimmick();

	//ハンマーの生成
	modelHammer.reset(Model::CreateFromOBJ("hammer", true));
	hammer = std::make_unique<Hammer>();
	hammer->Initialize(modelHammer.get(),viewProjection);
	hammer->SetParent(this->GetWorldTransform()[kBody]);

	//ヒットエフェクトの生成

	modelEffect_.reset(Model::CreateSphere());
	hitEffect_ = std::make_unique<HitEffect>();
	hitEffect_->Initialize(modelEffect_.get(), viewProjection_);
	hitEffect_->SetParent(this->GetWorldTransform()[kBody]);


	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));

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
}

// 更新処理
void Player::Update() 
{ 
	ApplyGlobalVariables();

	if (isHit_ && hitEffect_) {
		hitEffect_->Update();
	}

	InitializeBehavior();
	UpdateBehavior();

#ifdef _DEBUG
	DragFloat3("player.translate", &worldTransforms_[kBase]->translation_.x, 0.01f);
	DragInt("parameter", &workAttack_.attackParameter_, 0.01f);
	DragInt("combo", &workAttack_.comboIndex, 0.01f);
	Checkbox("Hit", &isHit_);
#endif // _DEBUG

}

// 描画処理
void Player::Draw() { 

	// 3Dモデルを描画
	models_[kBody]->Draw(*worldTransforms_[kBody], *viewProjection_);           // 体
	models_[kHead]->Draw(*worldTransforms_[kHead], *viewProjection_);           // 頭
	models_[kLeft_arm]->Draw(*worldTransforms_[kLeft_arm], *viewProjection_);   // 左腕
	models_[kRight_arm]->Draw(*worldTransforms_[kRight_arm], *viewProjection_); // 右腕

	//ふるまいが攻撃の時のみ
	if (behavior_ == Behavior::kAttack) {
		//近接武器(ハンマー)を描画する
		hammer->Draw();
	}

	if (isHit_ && hitEffect_) {
		hitEffect_->Draw();
	}

}

// 衝突時処理
void Player::OnCollision([[maybe_unused]] Collider* other) { 
	uint32_t typeID = other->GetTypeID();
	
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::KEnemy)) {
		// 衝突していれば、ジャンプ行動をリクエストする
		behaviorRequest_ = Behavior::kJump;
		//isHit_ = true;
	}
}

// 中心座標取得
Vector3 Player::GetCenterPosition() const {
	//ローカル座標でのオフセット
	const Vector3 offset = {0.f, 1.5f, 0.f};
	//ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransforms_[kBase]->matWorld_);
	return worldPos;
}

#pragma region 初期化処理メンバ関数の定義

// 各ワールド変換データの初期化
void Player::InitializeWorldTransform() {

	// 各パーツのワールド変換データを初期化する
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

// 攻撃行動初期化
void Player::BehaviorAttackInitialize() {
	worldTransforms_[kBody]->translation_.y = 0;
	worldTransforms_[kLeft_arm]->rotation_.x = -1.53f;
	worldTransforms_[kRight_arm]->rotation_.x = -1.53f;
	hammer->SetRotation({.x = 3.0f});
	

	workAttack_.attackParameter_ = 0;
}

// ダッシュ行動初期化
void Player::BehaviorDashInitialize() {
	workDash_.dashParameter_ = 0;
	worldTransforms_[kBase]->rotation_.y = destinationAngleY;
	worldTransforms_[kLeft_arm]->rotation_.x = 0.5f;
	worldTransforms_[kRight_arm]->rotation_.x = 0.5f;
}

// ジャンプ行動初期化
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

void (Player::*Player::behaviorInitializeTable[])(){ 
	&Player::BehaviorRootInitialize,
	&Player::BehaviorAttackInitialize,
	&Player::BehaviorDashInitialize,
	&Player::BehaviorJumpInitialize
};

#pragma endregion

#pragma region 更新処理メンバ関数の定義

// ゲームパッド操作
bool Player::GamePadController() { 
	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {
		const float deadZone = 0.7f;

		velocity_ = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};

		if (Calculator::Length(velocity_) > deadZone) {
			return true;
		}
	}

	return false;
}

// ジョイスティックによる移動
void Player::JoyStickMove(const float speed) {

	if (this->GamePadController()) {
		isMoving = true;
	} else {
		isMoving = false;
	}

	if (isMoving) {

		velocity_ = ~velocity_ * speed;

		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);

		velocity_ = TransformNormal(velocity_, rotateYMatrix);

		worldTransforms_[kBase]->translation_ += velocity_;

		targetRotate_.y = std::atan2(velocity_.x, velocity_.z);
	} else if (lockOn_ && lockOn_->ExistTarget()) {
		// ロックオン対象の座標取得
		Vector3 lockOnPos = lockOn_->GetTargetPosition();
		// 追従対象からロックオン対象へのベクトルを求める
		Vector3 sub = lockOnPos - this->GetCenterPosition();
		// Y軸周り角度
		worldTransforms_[kBase]->rotation_.y = std::atan2(sub.x, sub.z);
	}

	worldTransforms_[kBase]->rotation_.y = LerpShortAngle(
		worldTransforms_[kBase]->rotation_.y, targetRotate_.y, destinationAngleY);
	
}

// 浮遊ギミック
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

//通常行動
void Player::BehaviorRootUpdate() {

	XINPUT_STATE joyState;
	const float speed = 0.3f;

	JoyStickMove(speed);
	UpdateFloatingGimmick();

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

//攻撃行動
void Player::BehaviorAttackUpdate() {

	if (lockOn_ && lockOn_->ExistTarget()) {
		// ロックオン対象の座標取得
		Vector3 lockOnPos = lockOn_->GetTargetPosition();
		// 追従対象からロックオン対象へのベクトルを求める
		Vector3 sub = lockOnPos - this->GetCenterPosition();
		// 距離
		float distance = Rendering::Length(sub);
		// 距離しきい値
		const float threshold = 0.2f;
		// しきい値より離れている時のみ
		if (distance > threshold) {
			// Y軸周り角度
			worldTransforms_[kBase]->rotation_.y = std::atan2(sub.x, sub.z);

			// しきい値を超える速さなら補正する
			if (speed_ > distance - threshold) {
				// ロックオン対象へのめり込み防止
				speed_ = distance - threshold;
			}
		}
	}

	workAttack_.attackParameter_++;

	if (workAttack_.attackParameter_ > 0 && workAttack_.attackParameter_ < 10) {
		worldTransforms_[kLeft_arm]->rotation_.x -= kConstAttacks_[1].anticipationSpeed;
		worldTransforms_[kRight_arm]->rotation_.x -= kConstAttacks_[1].anticipationSpeed;
		Vector3 hammerAngle = hammer->GetRotation(); // ハンマーの回転
		hammerAngle.x -= kConstAttacks_[1].anticipationSpeed * 1.65f;
		hammer->SetRotation(hammerAngle);
	}

	if (workAttack_.attackParameter_ > 15 && workAttack_.attackParameter_ < 25) {

		Vector3 forward = Rendering::TransformNormal({0, 0, 1}, worldTransforms_[kBase]->matWorld_);
		worldTransforms_[kBase]->translation_ += forward * kConstAttacks_[1].chargeSpeed;
	}

	if (workAttack_.attackParameter_ > 30 && workAttack_.attackParameter_ < 40) {
		worldTransforms_[kLeft_arm]->rotation_.x += kConstAttacks_[1].swingSpeed;
		worldTransforms_[kRight_arm]->rotation_.x += kConstAttacks_[1].swingSpeed;
		Vector3 hammerAngle = hammer->GetRotation();
		hammerAngle.x += kConstAttacks_[1].swingSpeed;
		hammer->SetRotation(hammerAngle);
	}
    
	int32_t totalAttackTime = kConstAttacks_[1].anticipationTime + 
		kConstAttacks_[1].chargeTime + kConstAttacks_[1].swingTime + kConstAttacks_[1].recoveryTime;

	if (workAttack_.attackParameter_ >= totalAttackTime) {
		worldTransforms_[kBody]->rotation_.y = 0.0f;
		workAttack_.attackParameter_ = 0;
		workAttack_.comboIndex = 0;
		behaviorRequest_ = Behavior::kRoot;
	}

};

//ダッシュ行動
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

//ジャンプ行動
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

// ふるまい更新
void Player::UpdateBehavior() {

	hammer->Update();

	//ふるまい更新をメンバ関数ポインタで呼び出す
	(this->*behaviorUpdateTable[static_cast<size_t>(behavior_)])();

	// 行列を更新する
	for (auto worldTransform : worldTransforms_) {
		worldTransform->UpdateMatrix();
	}
}

void (Player::*Player::behaviorUpdateTable[])(){ 
	&Player::BehaviorRootUpdate,
	&Player::BehaviorAttackUpdate,
	&Player::BehaviorDashUpdate,
	&Player::BehaviorJumpUpdate
};

#pragma endregion

// デバッグテキストの描画
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
	amplitube = globalVariables->GetFloatValue(groupName, "FloatingAmplitube");
	armAngle_ = globalVariables->GetFloatValue(groupName, "idelArmAngleMax");

}

