#pragma once

#include <vector>
#include "objects/unit/Unit.h"
#include "objects/building/Building.h"
#include "objects/building/BuildingFactory.h"
#include "objects/unit/UnitFactory.h"
#include "objects/resource/ResourceFactory.h"
#include "SimulationInfo.h"
#include "scene/load/dbload_container.h"
#include "hud/window/main_menu/new_game/NewGamePlayer.h"

using namespace std;

class SimulationObjectManager
{
public:
	SimulationObjectManager();
	~SimulationObjectManager();

	vector<Unit*>* getUnits();
	vector<Building*>* getBuildings();
	vector<ResourceEntity*>* getResources();

	void addUnits(unsigned number, int id, Vector3* center, int player);
	void addBuildings(int id, Vector3* center, int player, IntVector2 _bucketCords);
	void addResources(int id, Vector3* center, IntVector2 _bucketCords);

	void clean();
	void updateInfo(SimulationInfo* simulationInfo);
	void dispose();

	void load(dbload_unit* unit);
	void load(dbload_building* building);
	void load(dbload_resource_entities* resource);

private:

	void updateUnits();
	void updateBuilding();
	void updateResource();

	bool shouldDelete(Physical* physical);

	void add(Unit* unit);
	void add(Building* building);
	void add(ResourceEntity* resourceEntity);

	void addAll(vector<Unit*>* _units);
	void addAll(vector<Building*>* _buildings);
	void addAll(vector<ResourceEntity*>* _resources);

	UnitFactory unitFactory;
	BuildingFactory buildingFactory;
	ResourceFactory resourceFactory;

	SimulationInfo* simulationInfo;

	vector<Unit*>* units;
	vector<Building*>* buildings;
	vector<ResourceEntity*>* resources;
	vector<Physical*> toDispose;

	vector<Building*>* buildingsToAdd;
	vector<ResourceEntity*>* resourcesToAdd;

	vector<Unit*>* unitsTemp;
	vector<Building*>* buildingsTemp;
	vector<ResourceEntity*>* resourcesTemp;
};
