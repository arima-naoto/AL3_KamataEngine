#pragma once
class ViewProjection;

class FollowCamera {

public:

	void Initialize(ViewProjection *viewProjection);

	void Update();

private:

	ViewProjection* viewProjection_ = nullptr;

};
