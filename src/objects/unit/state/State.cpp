#include "State.h"
#include "../Unit.h"

State::State() {
	std::fill_n(transitions, STATE_SIZE, false);
}

State::~State() = default;

void State::onEnd(Unit* unit) {
	unit->atState = false;
}

void State::execute(Unit* unit) {
	unit->atState = true;
}

bool State::validateTransition(UnitStateType stateTo) {
	return transitions[static_cast<char>(stateTo)];
}
