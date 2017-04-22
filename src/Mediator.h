#pragma once
#include "EnviromentStrategy.h"
#include "Controls.h"

class Mediator
{
public:
	Mediator(EnviromentStrategy* _enviromentStrategy, Controls* _controls);
	~Mediator();
	std::vector<Entity *>* getEntities(std::pair<Entity*, Entity*>* held);
private:
	EnviromentStrategy* enviromentStrategy;
	Controls* controls;
};