#define NOMINMAX
#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "imgui.h"
#include "algorithm"
#include "PlayerBullet.h"
#include "Rendering.h"

Player::~Player() {
	for (auto* bullet : bullets_) {
		delete bullet;
	}
	
}

void Player::Initialize(Model* model, ViewProjection* viewProjection, uint32_t textureHandle, const Vector3& position) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;//モデル

	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	
	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// テクスチャハンドル
	textureHandle_ = textureHandle; 
	
	input_ = Input::GetInstance();

	modelBullet_ = Model::CreateFromOBJ("playerBullet", true);
}

void Player::Update() 
{
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Player::MoveLimit();

	Player::Attack();

	for (auto* bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.translation_ += velocity_;

	//行列を更新する
	worldTransform_.UpdateMatrix();
}

void Player::Draw() 
{ 


	for (auto* bullet : bullets_) {
		bullet->Draw(*viewProjection_);
	}

	//3Dモデルを描画
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_); 

}

void Player::OnCollision() {}

void Player::MoveLimit() {

	// プレイヤーの移動範囲を設定する
	const float kLimitMoveX = 33;
	const float kLimitMoveY = 18;

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kLimitMoveX, kLimitMoveX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kLimitMoveY, kLimitMoveY);

}

#pragma region 移動処理メンバ関数の定義

void Player::MoveRight() { velocity_.x += kCharacterSpeed; }

void Player::MoveLeft() { velocity_.x -= kCharacterSpeed; }

void Player::MoveUp() { velocity_.y += kCharacterSpeed; }

void Player::MoveDown() { velocity_.y -= kCharacterSpeed; }

#pragma endregion

#pragma region 回転処理メンバ関数の定義

void Player::RotateRight() { parentRotation_.y -= kRotSpeed; }

void Player::RotateLeft() { parentRotation_.y += kRotSpeed; }

#pragma endregion 

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = Rendering::TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(modelBullet_, worldTransform_.translation_, velocity);

		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のTx
	worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のTy
	worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のTz

	return worldPos;

}

AABB Player::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth_ / 2.0f, worldPos.y - kWidth_ / 2.0f, worldPos.z - kWidth_ / 2.0f};
	aabb.max = {worldPos.x + kHeight_ / 2.0f, worldPos.y + kHeight_ / 2.0f, worldPos.z + kHeight_ / 2.0f};

	return aabb;
}

const std::list<PlayerBullet*>& Player::GetBullets() const { return bullets_; }

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

Vector3 Player::GetWorldTranslate() { return parentTranslation_; }

Vector3 Player::GetWorldRotation() { return parentRotation_; }