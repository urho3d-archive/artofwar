#include "FormationAction.h"
#include "Game.h"
#include "simulation/env/Enviroment.h"


FormationAction::FormationAction(Formation* formation, OrderType action, const Physical* physical, Vector2* vector,
                                 bool append)
	: ActionCommand(action, physical, vector, append) {
	this->formation = formation;
}

FormationAction::~FormationAction() = default;

void FormationAction::addTargetAim(Vector2* to, bool append) {
	auto opt = formation->getLeader();
	if (opt.has_value()) {
		ActionParameter parameter = getTargetAim(opt.value()->getBucketIndex(-1), *to, append);

		const auto id = static_cast<char>(action);
		opt.value()->action(id, parameter);
		Game::get()->getEnviroment()->invalidateCache();
	}
}

void FormationAction::addChargeAim(Vector2* charge, bool append) {
	auto opt = formation->getLeader();
	if (opt.has_value()) {
		const char id = static_cast<char>(action);

		ActionParameter parameter = getChargeAim(charge, append);
		opt.value()->action(id, parameter);
	}
}

void FormationAction::addFollowAim(const Physical* toFollow, bool append) {
	auto opt = formation->getLeader();
	if (opt.has_value()) {
		const char id = static_cast<char>(action);

		ActionParameter parameter = getFollowAim(toFollow, append);
		opt.value()->action(id, parameter);
	}
}
