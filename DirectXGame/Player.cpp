#define NOMINMAX
#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "imgui.h"
#include "algorithm"
#include "PlayerBullet.h"
#include "Rendering.h"
#include "TextureManager.h"
#include "WinApp.h"

Player::~Player() {
	for (auto* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();
	
	delete sprite2DReticle_;

}

void Player::Initialize(Model* model, ViewProjection* viewProjection, uint32_t textureHandle,const Vector3 &position) {

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
	//modelReticle_ = Model::CreateFromOBJ("cube", true);

	worldTransform3DReticle_.Initialize();

	textureReticle_ = TextureManager::Load("reticle.png");

	sprite2DReticle_ = Sprite::Create(textureReticle_, Vector2({640,600}), {1, 1, 1, 1}, {0.5f, 0.5f});
	sprite2DReticle_->SetSize(Vector2{90, 90});

}

void Player::Update() 
{

	Player::MoveLimit();

	Player::Attack();

	for (auto* bullet : bullets_) {
		bullet->Update();
	}

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Layout_3DReticle();

	worldTransform3DReticle_.UpdateMatrix();

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

	//modelReticle_->Draw(worldTransform3DReticle_, *viewProjection_);

}

void Player::DrawUI() {

	sprite2DReticle_->Draw();

}

void Player::OnCollision() {}

#pragma region 移動処理メンバ関数の定義

void Player::MoveRight() { velocity_.x += kCharacterSpeed; }

void Player::MoveLeft() { velocity_.x += -kCharacterSpeed; }

void Player::MoveUp() { velocity_.y += kCharacterSpeed; }

void Player::MoveDown() { velocity_.y += -kCharacterSpeed; }

#pragma endregion

#pragma region 回転処理メンバ関数の定義

void Player::RotateRight() { worldTransform_.rotation_.y -= kRotSpeed; }

void Player::RotateLeft() { worldTransform_.rotation_.y += kRotSpeed; }

#pragma endregion 

void Player::MoveLimit() {

	// プレイヤーの移動範囲を設定する
	const float kLimitMoveX = 33;
	const float kLimitMoveY = 18;

    worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kLimitMoveX, kLimitMoveX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kLimitMoveY, kLimitMoveY);
}

void Player::Attack() {

	if (input_->IsTriggerMouse(0)) {
		
		const float kBulletSpeed = 1.0f;

		Vector3 velocity{0, 0, kBulletSpeed};
		velocity = Get3DReticlePosition() - GetWorldPosition();
		velocity = Calculation::Normalize(velocity) * kBulletSpeed;
		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = Rendering::TransformNormal(velocity, worldTransform_.matWorld_);

		Vector3 bulletPosition = this->GetWorldPosition();

		// 生成
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(modelBullet_, bulletPosition, velocity);

		bullets_.push_back(newBullet);

	}
}

void Player::Layout_3DReticle() {

	const float kDistancePlayerTo3Dreticle = 50.0f;

	Vector3 offset = {0, 0, 1.0f};

	Matrix4x4 worldMatrix = Rendering::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	offset = offset * worldMatrix;

	offset = Calculation::Normalize(offset) * kDistancePlayerTo3Dreticle;

	worldTransform3DReticle_.translation_ = Vector3(offset.x, offset.y, offset.z);
	worldTransform3DReticle_.UpdateMatrix();

	Screen_Convert(offset.z);


}

void Player::Screen_Convert(float offsetZ) {

	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	Vector3 positonReticle = this->Get3DReticlePosition();

	Matrix4x4 matViewport = Rendering::ViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	//ビュープロジェクションビューポート行列
	Matrix4x4 matVPV = viewProjection_->matView * viewProjection_->matProjection * matViewport;
	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Rendering::Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3(float(mousePosition.x), float(mousePosition.y), 0);
	Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1);

	//スクリーン座標系からワールド座標系へ
	posNear = Rendering::Transform(posNear, matInverseVPV);
	posFar = Rendering::Transform(posFar, matInverseVPV);

	//マウスレイ
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Calculation::Normalize(mouseDirection);

	const float kDistanceTextObject = 50.0f;
	worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTextObject;
	worldTransform3DReticle_.translation_.z = offsetZ;

	worldTransform3DReticle_.UpdateMatrix();

	Matrix4x4 matViewProjectionViewport = viewProjection_->matView * viewProjection_->matProjection * matViewport;

	positonReticle = Rendering::Transform(positonReticle, matViewProjectionViewport);

	//マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(Vector2{float(mousePosition.x), float(mousePosition.y)});
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

Vector3 Player::Get3DReticlePosition() {

	Vector3 reticleWorldPos;
	reticleWorldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	reticleWorldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	reticleWorldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return reticleWorldPos;
}

AABB Player::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth_ / 2.0f, worldPos.y - kWidth_ / 2.0f, worldPos.z - kWidth_ / 2.0f};
	aabb.max = {worldPos.x + kHeight_ / 2.0f, worldPos.y + kHeight_ / 2.0f, worldPos.z + kHeight_ / 2.0f};

	return aabb;
}

const std::list<PlayerBullet*>& Player::GetBullets() const { return bullets_; }

void Player::SetParent(const WorldTransform* parent) {
	worldTransform_.parent_ = parent;
	translate_ = worldTransform_.parent_->translation_;
}

void Player::SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

Vector3 Player::GetWorldTranslate() { return translate_; }

Vector3 Player::GetWorldRotation() { return rotate_; }
