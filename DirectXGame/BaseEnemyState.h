#pragma once
#include <string>

class Enemy;

class BaseEnemyState {
public:

	BaseEnemyState(Enemy* enemy);

	virtual ~BaseEnemyState() = 0;

	virtual void Update() = 0;

protected:

	Enemy* enemy_ = nullptr;

};
