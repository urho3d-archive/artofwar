#include "ActionCommand.h"
#include "Game.h"
#include "objects/unit/ActionParameter.h"
#include "objects/unit/aim/ChargeAim.h"
#include "objects/unit/aim/DummyAim.h"
#include "objects/unit/aim/FollowAim.h"
#include "objects/unit/aim/TargetAim.h"
#include "simulation/env/Enviroment.h"
#include <chrono>


ActionCommand::ActionCommand(OrderType action, Physical* physical, Vector2* vector, bool append) {
	this->action = action;
	this->toFollow = physical;
	this->vector = vector;
	this->append = append;
}

ActionCommand::~ActionCommand() {
	delete vector;
}


ActionParameter ActionCommand::getTargetAim(int startInx, Vector2& to, bool append) {
	auto path = Game::get()->getEnviroment()->findPath(startInx, to);
	Aim* aim;
	if (!path->empty()) {
		aim = new TargetAim(*path);
	} else {
		aim = new DummyAim();
	}

	ActionParameter parameter(aim, append);
	return parameter;
}

ActionParameter ActionCommand::getFollowAim(Physical* toFollow, bool append) {
	ActionParameter parameter(new FollowAim(toFollow), append);
	return parameter;
}

ActionParameter ActionCommand::getChargeAim(Vector2* charge, bool append) {
	ActionParameter parameter(new ChargeAim(charge), append);
	return parameter;
}


void ActionCommand::execute() {
	switch (action) {
	case OrderType::GO:
		addTargetAim(vector, append);
		break;
	case OrderType::PATROL:
		addTargetAim(vector, append);
		break;
	case OrderType::FOLLOW:
		{
		if (toFollow != nullptr && toFollow->isAlive()) {
			addFollowAim(toFollow, append);
		}
		}
		break;
	case OrderType::CHARGE:
		{
		addChargeAim(vector, append);
		}
		break;
	default: ;
	}
}
