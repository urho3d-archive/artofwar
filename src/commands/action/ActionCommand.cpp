#include "ActionCommand.h"
#include "Game.h"
#include "objects/unit/ActionParameter.h"
#include "objects/unit/aim/ChargeAim.h"
#include "objects/unit/aim/DummyAim.h"
#include "objects/unit/aim/FollowAim.h"
#include "objects/unit/aim/TargetAim.h"
#include "simulation/env/Enviroment.h"
#include <chrono>


ActionCommand::ActionCommand(OrderType action, const Physical* physical, Vector2* vector, bool append) {
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
	if (!path->empty()) {
		return ActionParameter(new TargetAim(*path), append);
	}
	return ActionParameter(new DummyAim(), append);
}

ActionParameter ActionCommand::getFollowAim(const Physical* toFollow, bool append) {
	return ActionParameter(new FollowAim(toFollow), append);
}

ActionParameter ActionCommand::getChargeAim(Vector2* charge, bool append) {
	return ActionParameter(new ChargeAim(charge), append);
}


void ActionCommand::execute() {
	switch (action) {
	case OrderType::PATROL:
	case OrderType::GO:
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
		addChargeAim(vector, append);	
		break;
	default: ;
	}
}
