#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
class RailCamera {
public:
	void Initialize(const Matrix4x4& matWorld, const Vector3& rotate);
	void Update();
	const WorldTransform& GetWorldTransfrom() const;
	const ViewProjection& GetViewProjection() const;

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

}; 