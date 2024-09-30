#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "cassert"

#include "memory"
using namespace std;

#ifdef _DEBUG
#include <imgui.h>
using namespace ImGui;
#endif // _DEBUG


class IPlayerParts {

public:

#pragma region プレイヤーパーツの列挙体の宣言

	enum class Parts {
		kBody,
		kHead,
		kLeft_arm,
		kRight_arm,

		kPartsNum
	};

	using PartsName = Parts;
	static inline PartsName body = Parts::kBody;
	static inline PartsName head = Parts::kHead;
	static inline PartsName left_arm = Parts::kLeft_arm;
	static inline PartsName right_arm = Parts::kRight_arm;

	static inline const int partsNum = static_cast<int>(Parts::kPartsNum);

#pragma endregion 

public:

	IPlayerParts() = default;

	virtual ~IPlayerParts() = default;

	virtual void Initialize(Model* model,const ViewProjection *viewProjection) = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	const WorldTransform& GetWorldTransform() { return this->worldTransform; }

	void SetParent(const WorldTransform* parent) { worldTransform.parent_ = parent; }

	void SetPosition(const Vector3& position) { this->translate_ = position; }

	void SetAffine(){
		worldTransform.scale_ = scale_;
		worldTransform.rotation_ = rotate_;
		worldTransform.translation_ = translate_;
	}

protected:

	Model* model_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransform;

	Vector3 scale_ = {1.0f, 1.0f, 1.0f};

	Vector3 rotate_ = {};

	Vector3 translate_ = {};

};
