#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include <algorithm>

/// <summary>
/// Lerp関数の定義
/// </summary>
Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
	return {
		t* a.x + (1.0f - t) * b.x,
		t* a.y + (1.0f - t) * b.y,
		t* a.z + (1.0f - t) * b.z
	};
}

/// <summary>
/// コンストラクタ
/// </summary>
CameraController::CameraController() {}

/// <summary>
/// デストラクタ
/// </summary>
CameraController::~CameraController() {}

/// <summary>
/// 初期化処理
/// </summary>
void CameraController::Initialize() 
{
	//ビュープロジェクションの初期化
	viewProjection_.Initialize(); 
}

/// <summary>
/// リセット処理
/// </summary>
void CameraController::Reset() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;

}

/// <summary>
/// 更新処理
/// </summary>
void CameraController::Update() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットから目標座標を計算
	Vector3 targetVelocity = target_->GetVelocity();
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias_;
	
	//座標補間によりゆったり追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, targetPosition_, kIntrepolationRate);

	//追従対象の座標
	float positionX = targetWorldTransform.translation_.x;
	float positionY = targetWorldTransform.translation_.y;

	float viewProjectionX = viewProjection_.translation_.x;
	float viewProjectionY = viewProjection_.translation_.y;

	//追従対象が画面外に出ないように補間
	viewProjection_.translation_.x = std::clamp(viewProjectionX, positionX + margin_.left, positionX + margin_.right);
	viewProjection_.translation_.y = std::clamp(viewProjectionY, positionY + margin_.bottom, positionY + margin_.top);

	// 移動範囲制限
	viewProjection_.translation_.x = std::clamp(viewProjectionX, movableArea_.left, movableArea_.right);
	viewProjection_.translation_.y = std::clamp(viewProjectionY, movableArea_.bottom, movableArea_.top);

	//行列を更新する
	viewProjection_.UpdateMatrix();
}