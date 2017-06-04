#pragma once
#include <vector>
#include "Entity.h"
#include "AbstractCommand.h"
#include "Simulation.h"

class ActionCommand :public AbstractCommand
{
public:
	ActionCommand(std::vector<Entity*>* entities, ActionType action, Vector3* paremater);
	ActionCommand(Entity* entity, ActionType action, Vector3* paremater);
	~ActionCommand();
	void execute() override;
	void setAimConteiner(AimContainer* _aimContainer);
private:
	std::vector<Entity*>* entities;
	Entity* entity;
	ActionType action;

	AimContainer* aimContainer;
	Vector3* aimPosition;
};
