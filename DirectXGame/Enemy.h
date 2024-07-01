#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "MyStruct.h"

//Playerクラスを前方宣言する
class Player;

/// <summary>
/// 敵クラス
/// </summary>
class Enemy {
public://メンバ関数

	/// <summary>
	/// 度をラジアンに変換するメンバ関数を用意
	/// </summary>
	/// <param name="pi"></param>
	/// <param name="radian"></param>
	/// <returns></returns>
	float Lerp(float radian);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewprojection,const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="viewprojection"></param>
	void Draw();

	//衝突応答
	void OnCollision(Player* player);

public:

	Vector3 GetWorldPosition();

	AABB GetAABB();

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


};
