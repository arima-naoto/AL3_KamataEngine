#include "EnemyState.h"
#include "Enemy.h"
EnemyStateApproach::EnemyStateApproach(Enemy* enemy) : BaseEnemyState(enemy) {}

void EnemyStateApproach::Update() { enemy_->UpdateApproach(); }

EnemyStateLeave::EnemyStateLeave(Enemy* enemy) : BaseEnemyState(enemy) {}

void EnemyStateLeave::Update() { enemy_->UpdateLeave(); }
