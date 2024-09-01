#pragma once
#include "WorldTransform.h"
#include "MyStruct.h"

class Model;
class ViewProjection;
//Playerクラスを前方宣言する
class Player;

/// <summary>
/// 敵クラス
/// </summary>
class Enemy {
public://メンバ関数

	/// 度をラジアンに変換するメンバ関数を用意
	float Lerp(float radian);

	/// 初期化処理
	void Initialize(Model* model, ViewProjection* viewprojection,const Vector3& position);

	/// 更新処理
	void Update();

	/// 描画処理
	void Draw();

	/// 衝突応答
	void OnCollision(Player* player);

public:

	const WorldTransform& GetWorldTransform() { return worldTransform_; };

	Vector3 GetWorldPosition();

	AABB GetAABB();

	bool GetIsDead() const { return this->isDead_; }

	
	bool GetMoveTranslate() { return this->moveTranslate_; }
	bool SetMoveTranslate(bool moveTranslate) { return this->moveTranslate_ = moveTranslate; }

private://メンバ変数

	///ワールドトランスフォーム
	WorldTransform worldTransform_;

	///モデルデータ
	Model* model_;

	///ビュープロジェクション
	ViewProjection* viewProjection_;

	///歩行の速さ
	static inline const float kWalkSpeed = 0.05f;

	///速度
	Vector3 velocity_ = {};

	///最初の角度
	static inline const float kWalkMotionAngleStart = 0.0f;

	//最後の角度
	static inline const float kWalkMotionAngleEnd = 35.0f;

	///アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 1.5f;

	///経過時間
	float walkTimer_ = 0.0f;

	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;

	//衝突したときに座標が動くフラグ
	bool moveTranslate_ = false;
	
	
	bool isDead_ = false;

};
