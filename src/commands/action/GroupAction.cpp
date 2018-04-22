#include "GroupAction.h"
#include "Game.h"
#include "simulation/env/Enviroment.h"

GroupAction::GroupAction(std::vector<Physical*>* entities, OrderType action, Vector2* parameter, bool append)
	: ActionCommand(action, nullptr, parameter, append) {
	this->entities = entities;
}

GroupAction::GroupAction(std::vector<Physical*>* entities, OrderType action, Physical* paremeter, bool append)
	: ActionCommand(action, paremeter, nullptr, append) {
	this->entities = entities;
}

GroupAction::~GroupAction() = default;

void GroupAction::addTargetAim(Vector2* to, bool append) {
	auto opt = Game::get()->getFormationManager()->createFormation(entities);
	if (opt.has_value()) {
		if (append) {
			opt.value()->appendFutureTarget(*to, nullptr, action);
		} else {
			opt.value()->semiReset();
			opt.value()->addFutureTarget(*to, nullptr, action);
		}
	}
}

void GroupAction::addChargeAim(Vector2* charge, bool append) {
	auto opt = Game::get()->getFormationManager()->createFormation(entities);
	if (opt.has_value()) {
		if (append) {
			opt.value()->appendFutureTarget(*charge, nullptr, action);
		} else {
			opt.value()->semiReset();
			opt.value()->addFutureTarget(*charge, nullptr, action);
		}
	}
}

void GroupAction::addFollowAim(Physical* toFollow, bool append) {
	auto opt = Game::get()->getFormationManager()->createFormation(entities);
	if (opt.has_value()) {
		if (append) {
			opt.value()->appendFutureTarget(*(Vector2*)0, toFollow, action);
		} else {
			opt.value()->semiReset();
			opt.value()->addFutureTarget(*(Vector2*)0, toFollow, action);
		}
	}
}
