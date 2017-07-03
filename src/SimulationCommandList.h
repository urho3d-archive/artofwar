#pragma once
#include "CommandList.h"
#include "SimulationObjectManager.h"
#include "SimulationCommand.h"

class SimulationObjectManager;

class SimulationCommandList :public CommandList
{
public:
	SimulationCommandList(SimulationObjectManager * _simulationObjectManager);
	virtual ~SimulationCommandList();
protected:
	virtual void setParemeters(AbstractCommand* command) override;
private:
	SimulationObjectManager * simulationObjectManager;
};

