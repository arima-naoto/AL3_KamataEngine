#pragma once

#include "list"
#include "memory"
#include "Rendering.h"
#include "Vector2.h"

class Sprite;
class Enemy;
class ViewProjection;
class Input;

/// <summary>
/// ロックオン
/// </summary>
class LockOn : public Rendering {

public:
	/// 初期化処理
	void Initialize();

	/// 更新処理
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	///	描画処理
	void Draw();

private:
	// ターゲット検索
	void SearchTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	// スクリーン座標変換
	Vector3 ToScreen(Vector3 world, const ViewProjection& viewProjection);

	// 範囲外判定
	bool JudgmentOutScope(const ViewProjection& viewProjection);

private:
	uint32_t lockOnTexture_ = 0u;                 // ロックオンテクスチャ
	Sprite* lockOnMark_ = nullptr;                // ロックオンマーク用スプライト
	const Enemy* target_ = nullptr;               // ロックオン対象
	float kDegreeToRadian = 3.14f / 180.0f;       // しきい値
	float minDistance_ = 10.0f;                   // 最小距離
	float maxDistance_ = 40.0f;                   // 最大距離
	float angleRange_ = 20.0f * kDegreeToRadian; // 角度範囲
	Input* input_ = nullptr;                      // インプット
};
