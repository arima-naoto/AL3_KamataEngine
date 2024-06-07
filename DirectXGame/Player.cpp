#define NOMINMAX
#include "Player.h"
#include <numbers>
#include <Input.h>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

/// <summary>
/// インアウトサイン
/// </summary>
/// <param name="easing">イージング</param>
/// <returns></returns>
float Player::EaseInOutSine(float easing) { 
	return -(cosf(float(M_PI) * easing) - 1) / 2; 
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="model">3Dモデルデータ</param>
/// <param name="viewProjection">ビュープロジェクション</param>
/// <param name="position">三次元座標</param>
void Player::Initialize(Model* model,ViewProjection * viewProjection,const Vector3 &position) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;//モデル

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	
}

/// <summary>
/// 右の減速関数
/// </summary>
void Player::IsRightDecelerate() 
{
	// 移動中の右入力
	if (velocity_.x < 0.0f) {
		// 速度と逆方向に入力中は急ブレーキ
		velocity_.x *= (1.0f - kAttenuation);
	}
}

/// <summary>
/// 左の減速関数
/// </summary>
void Player::IsLeftDecelerate() 
{
	// 右移動中の左入力
	if (velocity_.x > 0.0f) {
		// 速度と逆方向に入力中は急ブレーキ
		velocity_.x *= (1.0f - kAttenuation);
	}
}

/// <summary>
/// 右の振り向き
/// </summary>
void Player::RightDirection()
{
	if (lrDirection_ != LRDirection::kRight) {
		lrDirection_ = LRDirection::kRight;
		turnFirstRotationY_ = worldTransform_.rotation_.y;
		turnTimer_ = kTimeTurn;
	}
}

/// <summary>
/// 左の振り向き
/// </summary>
void Player::LeftDirection() 
{
	if (lrDirection_ != LRDirection::kLeft) {
		lrDirection_ = LRDirection::kLeft;
		turnFirstRotationY_ = worldTransform_.rotation_.y;
		turnTimer_ = kTimeTurn;
	}
}

/// <summary>
/// 角度補間
/// </summary>
/// <param name="rotationY">角度Y</param>
/// <returns></returns>
void Player::RotateInterpolation(float rotationY) {

	float Turning = 1 + turnTimer_ / kTimeTurn;
	// 角度補間した変数にイージングをかける
	float easing = Player::EaseInOutSine(Turning);

	// 角度補間をする
	worldTransform_.rotation_.y = lerp(turnFirstRotationY_, rotationY, easing);
}

/// <summary>
/// 旋回制御
/// </summary>
void Player::TurningControl() 
{
	if (turnTimer_ > 0.0f) 
	{
		turnTimer_ -= 1.0f / 60.0f;

		// 左右の自キャラ角度テーブルの宣言と初期化
		float destinationRotationYTabel[] = {
		    std::numbers::pi_v<float> / 2.0f,
		    std::numbers::pi_v<float> * 3.0f / 2.0f,
		};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTabel[static_cast<uint32_t>(lrDirection_)];

		Player::RotateInterpolation(destinationRotationY);
	}
}

/// <summary>
/// ジャンプ制御
/// </summary>
void Player::JumpControl() 
{
	// 着地フラグ
	bool landing = false;
	// 着地減衰
	float kAttenuationLanding = 0.5f;

	// 地面との当たり判定
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}
	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 2.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuationLanding);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() 
{
	bool isRightBottom = Input::GetInstance()->PushKey(DIK_RIGHT);
	bool isLeftBottom = Input::GetInstance()->PushKey(DIK_LEFT);
	bool isUpBottom = Input::GetInstance()->PushKey(DIK_UP);

	if (onGround_) {

		// 移動入力
		// 左右移動操作
		if (isRightBottom || isLeftBottom) {
			// 左右加速
			Vector3 acceleration = {};
		
			if (isRightBottom) 
			{
				//右の減速
				Player::IsRightDecelerate();

				// 右の振り向き
				Player::RightDirection();

				// 右加速を加算する
				acceleration.x += kAcceleration;

			} else if (isLeftBottom) {

				//左の減速
				Player::IsLeftDecelerate();

				// 左の振り向き
				Player::LeftDirection();

				// 左加速を減算する
				acceleration.x -= kAcceleration;
			}
			// 加速/減速
			velocity_ += acceleration;
			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} 
		else {
			velocity_.x *= (1.0f - kAttenuation);
			if (velocity_.x * velocity_.x < 0.001f) {
				velocity_.x = 0;
			}
		}

		if (isUpBottom) {
			//ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}

	} else {
		//落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		//落花生原則後
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	//ジャンプしたときの処理や地面に接地したときの処理
	Player::JumpControl();
	// 旋回制御
	Player::TurningControl();
	//ワールドトランスフォームの座標に加速度を加算する
	worldTransform_.translation_ += velocity_;
	//行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}
	
/// <summary>
/// 描画処理
/// </summary>
void Player::Draw(const ViewProjection &viewProjection) 
{ 
	//3Dモデルを描画
	model_->Draw(worldTransform_,viewProjection); 
}