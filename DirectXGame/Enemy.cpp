#include "Enemy.h"
#include <cassert>
#include <numbers>
#define _USE_MATH_DEFINES
#include <cmath>

/// <summary>
/// 度をラジアンに変換する
/// </summary>
/// <param name="pi">円周率</param>
/// <param name="radian">角度</param>
/// <returns></returns>
float Enemy::Lerp(float radian)
{ 
	return radian * (3.14f / 180.0f);
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="model"></param>
/// <param name="viewProjection"></param>
void Enemy::Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受け取った初期座標をワールドトランスフォームに代入する
	worldTransform_.translation_ = position;
	//初期回転角を指定する
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

	//引数の内容をメンバ変数に記録する
	viewProjection_ = viewProjection;

	//速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void Enemy::Update() 
{ 
	//移動
	worldTransform_.translation_.x += velocity_.x;

	//タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	//円周率
	float pi = 3.14f;

	//サインカーブによる繰り返し
	float moveX = (2 * pi) * walkTimer_ / kWalkMotionTime;
	//回転アニメーション
	float param = std::sin(moveX);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = Enemy::Lerp(radian);


	//座標を元に行列の更新を行う
	worldTransform_.UpdateMatrix(); 
}

/// <summary>
/// 描画処理
/// </summary>
void Enemy::Draw(const ViewProjection&viewprojection) {
	model_->Draw(worldTransform_, viewprojection);
}

/// <summary>
/// 衝突応答の定義
/// </summary>
/// <param name="player"></param>
void Enemy::OnCollision(Player* player) { 
	(void)player;
}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のTx
	worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のTy
	worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のTz

	return worldPos;

}

AABB Enemy::GetAABB() {

	Vector3 worldPos = Enemy::GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;

}