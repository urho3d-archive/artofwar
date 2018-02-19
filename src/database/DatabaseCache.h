#pragma once
#include "sqlite3/sqlite3.h"
#include "db_strcut.h"
#include <vector>
#include "commands/action/ActionCommand.h"
using namespace std;

class DatabaseCache
{
public:
	void execute(const char* sql, int (* load)(void*, int, char**, char**));
	DatabaseCache();
	~DatabaseCache();
	db_unit* getUnit(int i);
	db_hud_size* getHudSize(int i);
	db_graph_settings* getGraphSettings(int i);
	db_building* getBuilding(int i);
	db_nation* getNation(int i);
	db_resource* getResource(int i);
	db_hud_vars* getHudVar(int i);
	db_order* getOrder(int i);
	db_map* getMap(int i);
	db_player_colors* getPlayerColor(int i);
	db_settings* getSettings();
	db_resolution* getResolution(int id);
	db_unit_level* getUnitLevel(int id, int level);

	std::vector<db_unit*>* getUnitsForBuilding(int id);
	std::vector<db_cost*>* getCostForUnit(int id);
	std::vector<db_cost*>* getCostForBuilding(int id);
	std::vector<db_order*>* getOrdersForUnit(int id);

	int getResourceSize();
	int getHudVarsSize();
	int getBuildingSize();
	int getUnitSize();
	int getOrdersSize();
	int getMapSize();
	int getPlayerColorsSize();
	int getNationSize();
	int getHudSizeSize();
	int getGraphSettingsSize();
	int getResolutionSize();

	void executeSingle(const char* sql);
	void setGraphSettings(int i, db_graph_settings* graphSettings);
	void setSettings(int i, db_settings* settings);

private:
	db_container* dbContainer;
	sqlite3* database;
};
