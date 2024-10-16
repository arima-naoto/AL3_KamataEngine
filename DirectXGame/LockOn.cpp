#include "LockOn.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Enemy.h"
#include "ViewProjection.h"
#include "Input.h"
#include "cmath"
#include "WinApp.h"

#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG

 
void LockOn::Initialize() {

	lockOnTexture_ = TextureManager::Load("Reticle.png");
	lockOnMark_ = Sprite::Create(lockOnTexture_, {0, 0});

	//インプット生成処理
	input_ = Input::GetInstance();

}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {
	
	XINPUT_STATE joyState;

	// 何も押されていなかったら
	if (!input_->GetJoystickState(0, joyState)) {
		return;
	}

	//ロックオン状態だったら
	if (target_) {
		//C.ロックオン解除処理

		// ロックオンボタンを押したら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			// ロックオンを外す
			target_ = nullptr;
		}
		// 範囲外判定
		else if (JudgmentOutScope(viewProjection)) {
			// ロックオンを外す
			target_ = nullptr;
		}

	} else {


		if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)) {
			// A.ロックオン対象の検索
			LockOn::SearchTarget(enemies, viewProjection);
		} 

	}

    //B.ロックオンマークの座標計算
	//  ロックオン継続
	if (target_) {
		// 敵のロックオン座標取得
		Vector3 positionWorld = target_->GetCenterPosition();
		// ワールド座標からスクリーン座標に変換
		Vector3 positionScreen = ToScreen(positionWorld, viewProjection);
		// Vector2変数に格納
		Vector2 positionScreenV2(positionScreen.x - 20, positionScreen.y - 20);
		// スプライトの座標を設定
		lockOnMark_->SetPosition(positionScreenV2);
	}
	
}

void LockOn::Draw() {
	if (target_) {
		lockOnMark_->Draw();
	}
}

void LockOn::SearchTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {

	// ①ロックオン対象の絞り込み

	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// 全ての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {

		// 敵のロックオン座標取得
		Vector3 positionWorld = enemy->GetCenterPosition();

		// ワールド->ビュー座標変換
		Vector3 positionView = Transform(positionWorld, viewProjection.matView);

		// 距離条件チェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

			// カメラ前方との角度を計算
			float arcTangent = std::atan2f(std::sqrtf(std::powf(positionView.x, 2) + std::powf(positionView.y, 2)), positionView.z);

			// 角度条件チェック(コーンに収まっているか)
			if (std::fabsf(arcTangent) <= angleRange_) {
				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		}
	}

	// ②距離でソートしてロックオン

	// ロックオン対象をリセット
	target_ = nullptr;
	if (!targets.empty()) {
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
		// ソートの結果一番近い敵をロックオン対象とする
		target_ = targets.front().second;
	}
}

Vector3 LockOn::ToScreen(Vector3 world, const ViewProjection& viewProjection) {

	Matrix4x4 matViewport = ViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Matrix4x4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * matViewport;

	world = Transform(world, matViewProjectionViewport);
		
	return world;
}

bool LockOn::JudgmentOutScope(const ViewProjection& viewProjection) {

	//敵のロックオン座標取得
	Vector3 positionWorld = target_->GetCenterPosition();
	//ワールドからビュー座標変換
	Vector3 positionView = Transform(positionWorld, viewProjection.matView);
	//距離条件チェック
	//  距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

		// カメラ前方との角度を計算
		float arcTangent = std::atan2f(std::sqrtf(
			std::powf(positionView.x, 2) + std::powf(positionView.y, 2)), positionView.z);
		//角度条件チェック(コーンに収まっているか)
		if (std::fabsf(arcTangent) <= angleRange_) {
			//範囲外ではない
			return false;
		}
	}

	//範囲外である
	return true;
}
