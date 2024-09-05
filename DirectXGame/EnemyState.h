#pragma once
#include "BaseEnemyState.h"
class EnemyStateApproach : public BaseEnemyState {

public:

	EnemyStateApproach(Enemy* enemy);

	void Update();

};

class EnemyStateLeave : public BaseEnemyState {

public:
	EnemyStateLeave(Enemy* enemy);

	void Update();
};
