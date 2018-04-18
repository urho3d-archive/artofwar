#include "AttackState.h"
#include "../Unit.h"


AttackState::AttackState() {
	nextStates[static_cast<char>(UnitStateType::STOP)] = true;
	nextStates[static_cast<char>(UnitStateType::DEFEND)] = true;
	nextStates[static_cast<char>(UnitStateType::DEAD)] = true;
	nextStates[static_cast<char>(UnitStateType::GO)] = true;
	nextStates[static_cast<char>(UnitStateType::FOLLOW)] = true;
	nextStates[static_cast<char>(UnitStateType::CHARAGE)] = true;
}


AttackState::~AttackState() = default;

void AttackState::onStart(Unit* unit) {
	unit->velocity = Urho3D::Vector2::ZERO;
}

void AttackState::onStart(Unit* unit, ActionParameter& parameter) {
}

void AttackState::onEnd(Unit* unit) {
	State::onEnd(unit);
	unit->enemyToAttack = nullptr;
}

void AttackState::execute(Unit* unit) {
	State::execute(unit);
	if (unit->enemyToAttack != nullptr && unit->enemyToAttack->isAlive()) {
		unit->velocity = Urho3D::Vector2::ZERO;
		unit->enemyToAttack->absorbAttack(unit->attackCoef);
	} else {
		unit->enemyToAttack = nullptr;
		StateManager::get()->changeState(unit, UnitStateType::STOP);
	}
}
