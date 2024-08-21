#define NOMINMAX
#include "Player.h"
#include <numbers>
#include <Input.h>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include <MapChipField.h>
#include "DebugText.h"
#include "Fade.h"
#include "GameScene.h"
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

#pragma region 移動用メンバ関数の定義

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

#pragma endregion

#pragma region 衝突判定用メンバ関数の定義

/// <summary>
/// 指定した角の座標計算
/// </summary>
/// <param name="center"></param>
/// <param name="corner"></param>
/// <returns></returns>
Vector3 Player::CornerPosition(const Vector3& center, Corner corner) 
{ 
	Vector3 offsetTable[kNumCorner] = {
	    {+kwidth / 2.0f, -kheight / 2.0f, 0}, //  kRightBottom
	    {-kwidth / 2.0f, -kheight / 2.0f, 0}, //  kLeftbottom
	    {+kwidth / 2.0f, +kheight / 2.0f, 0}, //  kRightTop
	    {-kwidth / 2.0f, +kheight / 2.0f, 0}, //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

/// <summary>
/// マップ衝突判定上方向
/// </summary>
/// <param name="info"></param>
void Player::MapCollisionTop(CollisionMapInfo& info) {
	
	// 上昇あり？
	if (info.move.y <= 0) {
		return;
	}

	//移動後の四つの角の座標
	array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew; 

	for (uint32_t index = 0; index < positionsNew.size(); index++) {
		positionsNew[index] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(index));
	}

	MapChipType mapChipType;

	//真上の当たり判定を行う
	bool hit = false;
	//左上点の判定(kLeftTopについて判定する)
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	
	//右上点の判定(kRightBottomについて判定する)
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//ブロックにヒット?
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move);
		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float breadth = rect.bottom - worldTransform_.translation_.y;
		float margin = kBlank + 2.0f;

		float moveY = breadth - margin;

		// Y移動量の更新
		info.move.y = std::max(0.0f, moveY);

		// 天井に当たったことを記録する
		info.isCeiling = true;
	}
}

/// <summary>
/// マップ衝突判定下方向
/// </summary>
/// <param name="info"></param>
void Player::MapCollisionBottom(CollisionMapInfo& info) {

	// 下降なし？
	if (info.move.y >= 0) {
		return;
	}

	// 移動後の四つの角の座標
	array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;

	for (uint32_t index = 0; index < positionsNew.size(); index++) {
		positionsNew[index] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(index));
	}

	MapChipType mapChipType;

	// 真下の当たり判定を行う
	bool hit = false;
	// 左下点の判定 (kLeftBottomについて判定する)
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定 (kRightBottomについて判定する)
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット?
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move);
		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float moveY = 0;

		// Y移動量の更新
		info.move.y = std::max(0.0f, moveY);

		// 床に当たったことを記録する
		info.isRanding = true;
	}
}

/// <summary>
/// マップ衝突判定右方向
/// </summary>
/// <param name="info"></param>
void Player::MapCollisionRight(CollisionMapInfo& info) {
	// 右方向への移動がない場合
	if (info.move.x <= 0) {
		return;
	}

	// 移動後の四つの角の座標
	array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;

	for (uint32_t index = 0; index < positionsNew.size(); index++) {
		positionsNew[index] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(index));
	}

	MapChipType mapChipType;

	// 右方向の当たり判定を行う
	bool hit = false;
	// 右上点の判定 (kRightTopについて判定する)
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定 (kRightBottomについて判定する)
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット?
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move);
		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float breadth = rect.left - worldTransform_.translation_.x;
		float margin = kBlank + 2.0f;

		float moveX = breadth - margin;

		// X移動量の更新
		info.move.x = std::max(0.0f, moveX);
		// 地面に当たってことを記録する
		info.ishitWall = true;
	}
}

/// <summary>
/// マップ衝突判定左方向
/// </summary>
/// <param name="info"></param>
void Player::MapCollisionLeft(CollisionMapInfo& info) {
	// 左方向の移動か？
	if (info.move.x >= 0) {
		return;
	}

	// 移動後の4つの角の座標の計算
	array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;
	for (uint32_t index = 0; index < positionsNew.size(); index++) {
		positionsNew[index] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(index));
	}

	MapChipType mapChipType;
	// 左の当たり判定を行う
	bool hit = false;
	// 左上点の判定(kLeftTopについて判定する)
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 左下点の判定(kLeftBottomについて判定する)
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量に設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move);
		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float breadth = rect.left - worldTransform_.translation_.x;
		float margin = kBlank / 2.0f;
		// Y移動量を求める
		float moveX = breadth + margin;
		info.move.x = std::max(0.0f, moveX);
		// 地面に当たってことを記録する
		info.ishitWall = true;
	}
}

/// <summary>
/// マップ衝突判定
/// </summary>
/// <param name="info"></param>
void Player::MapCollision(CollisionMapInfo& info) 
{ 
	//マップ衝突判定上方向メンバ関数の呼び出し
	Player::MapCollisionTop(info); 

	//マップ衝突判定下方向メンバ関数の呼び出し
	Player::MapCollisionBottom(info);
	
	//マップ衝突判定右方向メンバ関数の呼び出し
	Player::MapCollisionRight(info);
	
	//マップ衝突判定左方向メンバ関数の呼び出し
	Player::MapCollisionLeft(info);
}

/// <summary>
/// 判定結果反映移動
/// </summary>
/// <param name="info"></param>
void Player::ReflectMove(const CollisionMapInfo& info) 
{
	//移動
	worldTransform_.translation_ += info.move;
}

/// <summary>
/// 天井衝突時の処理
/// </summary>
/// <param name="info"></param>
void Player::CeilingContact(const CollisionMapInfo& info) {
	//天井に当たった？
	if (info.isCeiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

/// <summary>
/// 接地状態の処理
/// </summary>
/// <param name="info"></param>
void Player::RandingState(const CollisionMapInfo& info) {
	// ジャンプ開始
	if (velocity_.y > 0.0f) {
		onGround_ = false;
	} else {
		// 移動後の4つの角の座標の計算
		array<Vector3, static_cast<uint32_t>(Corner::kNumCorner)> positionsNew;
		for (uint32_t index = 0; index < positionsNew.size(); index++) {
			positionsNew[index] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(index));
		}

		MapChipType mapChipType;
		// 真下の当たり判定を行う
		bool hit = false;

		// 左下点の判定(kLeftBottom + Vector3(0, -kBlank, 0)について判定する)
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kBlank, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}

		// 右下点の判定(kRightBottom + Vector3(0, -kBlank, 0)について判定する)
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kBlank, 0));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}

		// 落下開始
		if (!hit) {
			// 空中処理に切り替える
			onGround_ = false;
		} else {
			// 着地処理
			onGround_ = true;
			velocity_.y = 0.0f; // 垂直速度をゼロにする
		}
	}
}

/// <summary>
/// 状態切り替えの処理
/// </summary>
/// <param name="info"></param>
void Player::StateSwitching(const CollisionMapInfo& info) 
{
	//自キャラが接地状態?
	if (onGround_) {
	
		//接地状態の処理
		Player::RandingState(info);

	} else {
		
		//空中状態の処理

		// 着地フラグ
		if (info.isRanding) {
			// 着地状態に切り替える(落下を止める)
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}

/// <summary>
/// 壁接触時処理
/// </summary>
/// <param name="inifo"></param>
void Player::WallContact(const CollisionMapInfo& info) {
	//壁接触による減速
	if (info.ishitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

#pragma endregion

/// <summary>
/// 更新処理
/// </summary>
void Player::Update() 
{
#pragma region プレイヤーの挙動

	//==============================================<①移動入力>==========================================================
	
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
		//落下制限速度
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	//ジャンプしたときの処理や地面に接地したときの処理
	Player::JumpControl();

	//==========================================<②移動量を加味して衝突判定する>==========================================

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	// マップ衝突チェック
	Player::MapCollision(collisionMapInfo);

	//==========================================<③判定結果を反映して移動させる>=========================================
	Player::ReflectMove(collisionMapInfo);

	//=============================================<④天井に接触している場合>============================================
	Player::CeilingContact(collisionMapInfo);

	//============================================<⑤壁に接触している場合の処理>=========================================
	Player::WallContact(collisionMapInfo);

	//================================================<⑥接地状態の切り替え>=============================================
	Player::StateSwitching(collisionMapInfo);

	//ワールドトランスフォームの座標に加速度を加算する
	//worldTransform_.translation_ += velocity_;

	// 旋回制御
	Player::TurningControl();

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

#pragma endregion 
}
	
/// <summary>
/// 描画処理
/// </summary>
void Player::Draw() {

	//死亡フラグがtrueになってなければ
	if (isDead_ != false) {
		return;
	}

	// 自キャラの3Dモデルを描画する
	model_->Draw(worldTransform_, *viewProjection_);
}

void Player::OnCollision(Enemy* enemy) { 
	(void)enemy;

	isDead_ = true;
}

/// <summary>
/// ワールド座標を取得
/// </summary>
/// <returns></returns>
Vector3 Player::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];//ワールド行列のTx
	worldPos.y = worldTransform_.matWorld_.m[3][1];//ワールド行列のTy
	worldPos.z = worldTransform_.matWorld_.m[3][2];//ワールド行列のTz

	return worldPos;

}

/// <summary>
/// AABBを取得
/// </summary>
/// <returns></returns>
AABB Player::GetAABB() 
{

	Vector3 worldPos = Player::GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x -  kwidth / 2.0f, worldPos.y -  kheight / 2.0f, worldPos.z -  kwidth / 2.0f};
	aabb.max = {worldPos.x +  kwidth / 2.0f, worldPos.y +  kheight / 2.0f, worldPos.z +  kwidth / 2.0f};

	return aabb;

}
