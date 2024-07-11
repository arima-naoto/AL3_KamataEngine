#include "Fade.h"
#include "TextureManager.h"
#include "algorithm"

Fade::Fade() {}

Fade::~Fade() { 
	delete sprite_; 
}

void Fade::Initialize() {

	textureHandle_ = TextureManager::Load("FadeBackGround.png");

	sprite_ = Sprite::Create(textureHandle_, {0, 0});

	sprite_->SetSize(Vector2(1280.0f, 720.0f));

	sprite_->SetColor(Vector4(0, 0, 0, 1));

}

void Fade::Start(Status status, float duration) {

	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;

}

void Fade::Stop() {

	status_ = Status::None;

}

bool Fade::IsFinished() const {
	//フェード状態の分岐
	switch (status_) {
	case Status::FadeIn:
	case Status::FadeOut:

		if (counter_ >= duration_) {
			return true;
		}
		else {
			return false;
		}

	}

	return false;
}

void Fade::UpdateFadeIn() {

	// 一フレーム分の秒数をカウントアップ
	counter_ += 1.0f / 60.0f;
	// フェード継続時間に達したら打ち止め
	if (counter_ >= duration_) {
		counter_ = duration_;
	}

	// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づく程アルファ値を小さくする
	sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1.0f - (counter_ / duration_), 0.0f, 1.0f)));


}

void Fade::UpdateFadeOut() {

	//一フレーム分の秒数をカウントアップ
	counter_ += 1.0f / 60.0f;
	//フェード継続時間に達したら打ち止め
	if (counter_ >= duration_) {
		counter_ = duration_;
	}

	//0.0fから1.0fの間で、経過時間がフェード継続時間に近づく程アルファ値を大きくする
	sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));


}

void Fade::Update() {

	//フェード状態のよる分岐
	switch (status_) {
	case Status::None:
		//何もしない
		break;
	case Status::FadeIn:

		///フェードイン中の更新処理
		Fade::UpdateFadeIn();

		break;
	case Status::FadeOut:

		///フェードアウト中の更新処理
		Fade::UpdateFadeOut();

		break;
	}

}

void Fade::Draw(ID3D12GraphicsCommandList* commandList) {

	if (status_ == Status::None) {
		return;
	}

	Sprite::PreDraw(commandList);
	sprite_->Draw();
	Sprite::PostDraw();

}