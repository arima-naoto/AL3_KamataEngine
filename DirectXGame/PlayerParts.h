#pragma once
#include "IPlayerParts.h"

class PlayerBody : public IPlayerParts {

public:
	PlayerBody() = default;

	~PlayerBody() override = default;

	void Initialize(Model* model, const ViewProjection* viewProjection) override;

	void Update() override;

	void Draw() override;

private:
	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

	float floatingParameter_ = 0.0f;
};

class PlayerHead : public IPlayerParts {

public:

	PlayerHead() = default;

	~PlayerHead() override = default;

	void Initialize(Model* model, const ViewProjection* viewProjection) override;

	void Update() override;

	void Draw() override;

};

class PlayerLeft_Arm : public IPlayerParts {

public:
	PlayerLeft_Arm() = default;

	~PlayerLeft_Arm() override = default;

	void Initialize(Model* model, const ViewProjection* viewProjection) override;

	void Update() override;

	void Draw() override;

private:

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

	float floatingParameter_ = 0.0f;
};

class PlayerRight_Arm : public IPlayerParts {

public:
	PlayerRight_Arm() = default;

	~PlayerRight_Arm() override = default;

	void Initialize(Model* model, const ViewProjection* viewProjection) override;

	void Update() override;

	void Draw() override;

private:

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();

	float floatingParameter_ = 0.0f;
};