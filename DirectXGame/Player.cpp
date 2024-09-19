#include "Player.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Rendering.h"

#include "cassert"
#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG


void Player::Initialize(Model* model, ViewProjection* viewProjection) {

	//NULLポインタチェック
	assert(model);
	
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model; // モデル

	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
	
	input_ = Input::GetInstance();

}

void Player::Update() 
{
	

	//ジョイスティックによる自機の移動
	Player::JoyStickMove();

	//行列を更新する
	worldTransform_.UpdateMatrix();
}

void Player::Draw() 
{ 
	//3Dモデルを描画
	model_->Draw(worldTransform_,*viewProjection_); 
}



void Player::JoyStickMove() {

	XINPUT_STATE joyState;

	if (input_->GetJoystickState(0, joyState)) {

		const float speed = 0.3f;
	   
		Vector3 move = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX ,0.f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};

		move = ~move * speed;

	    Matrix4x4 rotateYMatrix = Rendering::MakeRotateYMatrix(viewProjection_->rotation_.y);
	
		move = Rendering::TransformNormal(move, rotateYMatrix);

		worldTransform_.translation_ += move;

	    velocity_ = move;

		worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	}


}

void Player::SetViewProjection(const ViewProjection* viewProjection) {
	viewProjection_ = viewProjection;
}