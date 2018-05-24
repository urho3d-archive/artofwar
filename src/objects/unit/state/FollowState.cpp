#include "FollowState.h"
#include "../Unit.h"


FollowState::FollowState() {
	nextStates[static_cast<char>(UnitStateType::STOP)] = true;
	nextStates[static_cast<char>(UnitStateType::DEFEND)] = true;
	nextStates[static_cast<char>(UnitStateType::DEAD)] = true;
	nextStates[static_cast<char>(UnitStateType::GO)] = true;
	nextStates[static_cast<char>(UnitStateType::PATROL)] = true;
	nextStates[static_cast<char>(UnitStateType::MOVE)] = true;
}

FollowState::~FollowState() = default;

void FollowState::onStart(Unit* unit) {
}

void FollowState::onStart(Unit* unit, ActionParameter& parameter) {
	unit->clearAims();
	unit->addAim(parameter.aim);
	//TODO aim?
}


void FollowState::onEnd(Unit* unit) {
	State::onEnd(unit);
}

void FollowState::execute(Unit* unit) {
	State::execute(unit);
}
