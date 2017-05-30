#pragma once
#include "Entity.h"
#include <vector>
#include "AbstractCommand.h"
#include "SimulationObjectManager.h"
#include "ObjectEnums.h"

class SimulationCommand:public AbstractCommand
{
public:
	SimulationCommand(int _number, BuildingType _buildingType, Vector3* _position, SpacingType _spacingType, int _player);
	SimulationCommand(int _number, UnitType _unitType, Vector3* _position, SpacingType _spacingType, int _player);
	~SimulationCommand();
	virtual void execute() override;
	void setSimulationObjectManager(SimulationObjectManager* _simulationObjectManager);
private:
	SimulationObjectManager* simulationObjectManager;
	BuildingType buildingType;
	UnitType unitType;
	int number;
	int player;
	Vector3* position;
	ObjectType objectType;
	SpacingType spacingType;
};
