#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "vector"
#include "Collider.h"

class BaseCharacter : public Collider{

public:

	virtual void Initialize(std::vector<Model*> models, ViewProjection*viewProjection);

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual Vector3 GetCenterPosition() const override;

    const std::vector<WorldTransform*> GetWorldTransform() { return worldTransforms_; };

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection;}

protected:

	//モデルデータ配列
	std::vector<Model*> models_;

	//ワールド変換データ
	std::vector<WorldTransform*> worldTransforms_;

	const ViewProjection* viewProjection_ = nullptr;

};

