#pragma once
#include "Sprite.h"

/// <summary>
/// フェード
/// </summary>
class Fade {

public:

	// フェードの状態
	enum class Status {
		None,    // フェードなし
		FadeIn,  // フェードイン中
		FadeOut, // フェードアウト中
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Fade();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fade();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// フェード開始メンバ関数
	/// </summary>
	/// <param name="status">現在のフェード</param>
	/// <param name="duration">フェードの持続時間</param>
	void Start(Status status, float duration);

	///フェード停止メンバ関数
	void Stop();

	///フェード終了判定メンバ関数
	bool IsFinished() const; 

	/// <summary>
	/// フェードイン中の更新処理
	/// </summary>
	void UpdateFadeIn();

	/// <summary>
	/// フェードアウト中の更新処理
	/// </summary>
	void UpdateFadeOut();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList *commandList);

private: //メンバ関数

	///スプライトクラス
	Sprite* sprite_ = nullptr;

	///テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	///現在のフェードの状態
	Status status_ = Status::None;

	//フェードの持続時間
	float duration_ = 0.0f;

	//経過時間カウンター
	float counter_ = 0.0f;

};
