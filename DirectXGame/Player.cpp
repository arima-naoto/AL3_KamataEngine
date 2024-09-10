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

void Player::Initialize(Model* model, ViewProjection * viewProjection,uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;//モデル

	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	
	//ワールド変換の初期化
	worldTransform_.Initialize();

	// テクスチャハンドル
	textureHandle_ = textureHandle; 
	
	input_ = Input::GetInstance();

	modelBullet_ = Model::CreateFromOBJ("playerBullet", true);
}

void Player::Update() 
{

	// プレイヤーの移動範囲を設定する
	const float kLimitMoveX = 33;
	const float kLimitMoveY = 18;

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kLimitMoveX, kLimitMoveX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kLimitMoveY, kLimitMoveY);

	Player::Attack();

	for (auto* bullet : bullets_) {
		bullet->Update();
	}

	//行列を更新する
	worldTransform_.UpdateMatrix();
}

void Player::Draw() 
{ 
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (auto *bullet : bullets_) {
		bullet->Draw(*viewProjection_);
	}

	//3Dモデルを描画
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_); 

}

void Player::OnCollision() {}

#pragma region 移動処理メンバ関数の定義

void Player::MoveRight() {
	worldTransform_.translation_ += Vector3(kCharacterSpeed, 0, 0);
}

void Player::MoveLeft() {
	worldTransform_.translation_ -= Vector3(kCharacterSpeed, 0, 0);
}

void Player::MoveUp() { 
	worldTransform_.translation_ += Vector3(0, kCharacterSpeed, 0); 
}

void Player::MoveDown() { 
	worldTransform_.translation_ -= Vector3(0, kCharacterSpeed, 0); 
}

#pragma endregion

#pragma region 回転処理メンバ関数の定義

void Player::RotateRight() {
	worldTransform_.rotation_ -= Vector3(0, kRotSpeed, 0);
}

void Player::RotateLeft() {
	worldTransform_.rotation_+= Vector3(0, kRotSpeed, 0);
}

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

const std::list<PlayerBullet*>& Player::GetBullets() const { return bullets_; }

