﻿#pragma once
#define UNITS_NUMBER_DB 50
#define BUILDINGS_NUMBER_DB 50
#define HUD_SIZES_NUMBER_DB 4
#define GRAPH_SETTINGS_NUMBER_DB 4
#define TYPE_NUMBER_DB 50
#define NATION_NUMBER_DB 50
#define RESOURCE_NUMBER_DB 5
#define HUD_VARS_NUMBER_DB 50
#define ORDERS_NUMBER_DB 10
#define MAP_NUMBER_DB 10
#define PLAYER_COLORS_NUMBER_DB 10
#define RESOLUTIONS_NUMBER_DB 6
#define SETTINGS_NUMBER_DB 1
#define MAX_UNIT_LEVEL_NUMBER_DB 6
#define MAX_BUILDING_LEVEL_NUMBER_DB 6
#define PATH_UPGRADES_NUMBER_DB 30

#define SPLIT_SIGN '\n'

#include "defines.h"
#include <Urho3D/Container/Str.h>
#include <Urho3D/Math/Vector2.h>
#include <vector>


struct db_unit
{
	const int id;
	const Urho3D::String name;
	const bool rotatable;
	const int nation;
	const Urho3D::String icon;
	const int actionState;

	db_unit(int id, char* name, int rotatable, int nation, char* icon, int actionState)
		: id(id),
		name(name),
		rotatable(rotatable),
		nation(nation),
		icon(icon),
		actionState(actionState) {
	}
};

struct db_unit_level
{
	const int level;
	const int unit;
	const Urho3D::String name;
	const float minDist;
	const float maxSep;
	const Urho3D::String model;
	const Urho3D::String texture;
	const float mass;
	const float scale;
	const float attack;
	const int attackSpeed;
	const float attackRange;
	const float defense;
	const int maxHp;
	const float maxSpeed;
	const float minSpeed;
	const float collectSpeed;
	const float upgradeSpeed;
	const float maxForce;


	db_unit_level(int level, int unit, char* name, float minDist, float maxSep, char* model, char* texture,
	              float mass, float scale, float attack, int attackSpeed, float attackRange, float defense,
	              int maxHp, float maxSpeed, float minSpeed, float collectSpeed, float upgradeSpeed, float maxForce)
		: level(level),
		unit(unit),
		name(name),
		minDist(minDist),
		maxSep(maxSep),
		model(model),
		texture(texture),
		mass(mass),
		scale(scale),
		attack(attack),
		attackSpeed(attackSpeed),
		attackRange(attackRange),
		defense(defense),
		maxHp(maxHp),
		maxSpeed(maxSpeed),
		minSpeed(minSpeed),
		collectSpeed(collectSpeed),
		upgradeSpeed(upgradeSpeed),
		maxForce(maxForce) {
	}
};

struct db_unit_to_upgrade
{
	const int upgrade;
	const int unit;

	db_unit_to_upgrade(int upgrade, int unit)
		: upgrade(upgrade),
		unit(unit) {
	}
};

struct db_unit_upgrade
{
	const int id;
	const int path;
	const int level;
	const Urho3D::String name;
	const float attack;
	const float attackSpeed;
	const float attackRange;
	const float defense;
	const float maxHp;
	const float maxSpeed;
	const float minSpeed;
	const float collectSpeed;
	const float upgradeSpeed;
	Urho3D::String pathName;
	std::vector<db_unit*> units;

	db_unit_upgrade(int id, int path, int level, char* name, float attack, float attackSpeed, float attackRange,
	                float defense, float maxHp, float maxSpeed, float minSpeed, float collectSpeed, float upgradeSpeed)
		: id(id),
		path(path),
		level(level),
		name(name),
		attack(attack),
		attackSpeed(attackSpeed),
		attackRange(attackRange),
		defense(defense),
		maxHp(maxHp),
		maxSpeed(maxSpeed),
		minSpeed(minSpeed),
		collectSpeed(collectSpeed),
		upgradeSpeed(upgradeSpeed) {
	}
};

struct db_building
{
	const int id;
	const Urho3D::String name;
	const Urho3D::IntVector2 size;
	const int nation;
	const Urho3D::String icon;

	std::vector<int> unitUpgradePath;

	db_building(int id, char* name, int sizeX, int sizeZ, int nation, char* icon)
		: id(id),
		name(name),
		size(Urho3D::IntVector2(sizeX, sizeZ)),
		nation(nation),
		icon(icon) {
	}
};

struct db_building_level
{
	const int level;
	const int unit;
	const Urho3D::String name;
	const Urho3D::String model;
	const Urho3D::String texture;
	const float scale;
	const short queueMaxCapacity;


	db_building_level(int level, int unit, char* name, char* model, char* texture, float scale,
	                  short queueMaxCapacity)
		: level(level),
		unit(unit),
		name(name),
		model(model),
		texture(texture),
		scale(scale),
		queueMaxCapacity(queueMaxCapacity) {
	}
};

struct db_hud_size
{
	const int id;
	const Urho3D::String name;

	db_hud_size(int id, char* name)
		: id(id),
		name(name) {
	}
};

struct db_settings
{
	int graph;
	int resolution;

	db_settings(int graph, int resolution)
		: graph(graph),
		resolution(resolution) {
	}
};

struct db_resolution
{
	const int id;
	const int x;
	const int y;

	db_resolution(int id, int x, int y)
		: id(id),
		x(x),
		y(y) {
	}
};

struct db_graph_settings
{
	int id;
	int hud_size;
	Urho3D::Vector<Urho3D::String> styles;
	bool fullscreen;
	float max_fps;
	float min_fps;
	Urho3D::String name;
	bool v_sync;
	bool shadow;
	int texture_quality;

	db_graph_settings(int id, int hudSize, char* styles, int fullscreen, float maxFps, float minFps,
	                  char* name, bool v_sync, bool shadow, int texture_quality)
		: id(id),
		hud_size(hudSize),
		styles(Urho3D::String(styles).Split(SPLIT_SIGN)),
		fullscreen(fullscreen),
		max_fps(maxFps),
		min_fps(minFps),
		name(name),
		v_sync(v_sync),
		shadow(shadow),
		texture_quality(texture_quality) {
	}
};

struct db_nation
{
	const int id;
	const Urho3D::String name;

	db_nation(int id, char* name)
		: id(id),
		name(name) {
	}
};

struct db_resource
{
	const int id;
	const Urho3D::String name;
	const Urho3D::String icon;
	const int maxCapacity;
	const Urho3D::Vector<Urho3D::String> texture;
	const Urho3D::String model;
	const float scale;
	const Urho3D::IntVector2 size;
	const int maxUsers;
	const unsigned mini_map_color;

	db_resource(int id, char* name, char* icon, int maxCapacity, char* texture, char* model, float scale, int sizeX,
	            int sizeZ, int maxUsers, unsigned mini_map_color)
		: id(id),
		name(name),
		icon(icon),
		maxCapacity(maxCapacity),
		texture(Urho3D::String(texture).Split(SPLIT_SIGN)),
		model(model),
		scale(scale),
		size(Urho3D::IntVector2(sizeX, sizeZ)),
		maxUsers(maxUsers),
		mini_map_color(mini_map_color) {
	}
};

struct db_hud_vars
{
	const int id;
	const int hud_size;
	const Urho3D::String name;
	float value;

	db_hud_vars(int id, int hudSize, char* name, float value)
		: id(id),
		hud_size(hudSize),
		name(name),
		value(value) {
	}
};

struct db_cost
{
	const int id;
	const int resource;
	const int value;
	const Urho3D::String resourceName;
	const int thing;

	db_cost(int id, int resource, int value, Urho3D::String resourceName, int thing)
		: id(id),
		resource(resource),
		value(value),
		resourceName(resourceName),
		thing(thing) {
	}
};

struct db_order
{
	const int id;
	const Urho3D::String icon;
	const Urho3D::String name;

	db_order(int id, char* icon, char* name)
		: id(id),
		icon(icon),
		name(name) {
	}
};

struct db_map
{
	const int id;
	const Urho3D::String height_map;
	const Urho3D::String texture;
	const float scale_hor;
	const float scale_ver;
	const Urho3D::String name;

	db_map(int id, char* heightMap, char* texture, float scaleHor, float scaleVer, char* name)
		: id(id),
		height_map(heightMap),
		texture(texture),
		scale_hor(scaleHor),
		scale_ver(scaleVer),
		name(name) {
	}
};

struct db_player_colors
{
	const int id;
	const unsigned unit;
	const unsigned building;
	const Urho3D::String name;

	db_player_colors(int id, unsigned unit, unsigned building, char* name)
		: id(id),
		unit(unit),
		building(building),
		name(name) {
	}
};

struct db_container
{
	db_unit* units[UNITS_NUMBER_DB] = {nullptr};
	db_building* buildings[BUILDINGS_NUMBER_DB] = {nullptr};
	db_hud_size* hudSizes[HUD_SIZES_NUMBER_DB] = {nullptr};
	db_graph_settings* graphSettings[GRAPH_SETTINGS_NUMBER_DB] = {nullptr};
	db_nation* nations[NATION_NUMBER_DB] = {nullptr};
	db_resource* resources[RESOURCE_NUMBER_DB] = {nullptr};
	db_hud_vars* hudVars[HUD_VARS_NUMBER_DB] = {nullptr};
	db_order* orders[ORDERS_NUMBER_DB] = {nullptr};
	db_map* maps[MAP_NUMBER_DB] = {nullptr};
	db_player_colors* playerColors[PLAYER_COLORS_NUMBER_DB] = {nullptr};
	db_resolution* resolutions[RESOLUTIONS_NUMBER_DB] = {nullptr};
	db_settings* settings[SETTINGS_NUMBER_DB] = {nullptr};
	std::vector<db_unit_upgrade*>* unitUpgrades[PATH_UPGRADES_NUMBER_DB] = {nullptr};
	db_unit_upgrade* unitUpgradesPerId[PATH_UPGRADES_NUMBER_DB * 10] = {nullptr};
	std::vector<db_cost*>* unitUpgradesCosts[PATH_UPGRADES_NUMBER_DB * 10] = {nullptr};

	std::vector<db_unit*>* unitsForBuilding[BUILDINGS_NUMBER_DB]{};

	std::vector<db_cost*>* costForBuilding[BUILDINGS_NUMBER_DB]{};
	std::vector<db_cost*>* costForUnit[UNITS_NUMBER_DB]{};
	std::vector<std::vector<db_cost*>*>* costForBuildingLevel[BUILDINGS_NUMBER_DB]{};
	std::vector<std::vector<db_cost*>*>* costForUnitLevel[UNITS_NUMBER_DB]{};

	std::vector<db_order*>* ordersToUnit[UNITS_NUMBER_DB]{};

	std::vector<db_unit_level*>* levelsToUnit[UNITS_NUMBER_DB]{};
	std::vector<db_building_level*>* levelsToBuilding[BUILDINGS_NUMBER_DB]{};

	int units_size = 0;
	int hud_size_size = 0;
	int building_size = 0;
	int graph_settings_size = 0;
	int unit_type_size = 0;
	int resource_size = 0;
	int nation_size = 0;
	int hud_vars_size = 0;
	int orders_size = 0;
	int maps_size = 0;
	int player_colors_size = 0;
	int resolutions_size = 0;


	explicit db_container() {
		for (int i = 0; i < BUILDINGS_NUMBER_DB; ++i) {
			unitsForBuilding[i] = new std::vector<db_unit*>();
			unitsForBuilding[i]->reserve(DEFAULT_VECTOR_SIZE);
			costForBuilding[i] = new std::vector<db_cost*>();
			costForBuilding[i]->reserve(DEFAULT_VECTOR_SIZE);
			levelsToBuilding[i] = new std::vector<db_building_level*>();
			levelsToBuilding[i]->reserve(DEFAULT_VECTOR_SIZE);
			costForBuildingLevel[i] = new std::vector<std::vector<db_cost*>*>();
			costForBuildingLevel[i]->reserve(MAX_BUILDING_LEVEL_NUMBER_DB);
			for (int j = 0; j < MAX_BUILDING_LEVEL_NUMBER_DB; ++j) {
				costForBuildingLevel[i]->push_back(new std::vector<db_cost*>);
			}
		}
		for (int i = 0; i < UNITS_NUMBER_DB; ++i) {
			costForUnit[i] = new std::vector<db_cost*>();
			costForUnit[i]->reserve(DEFAULT_VECTOR_SIZE);
			ordersToUnit[i] = new std::vector<db_order*>();
			ordersToUnit[i]->reserve(DEFAULT_VECTOR_SIZE);
			levelsToUnit[i] = new std::vector<db_unit_level*>();
			levelsToUnit[i]->reserve(DEFAULT_VECTOR_SIZE);

			costForUnitLevel[i] = new std::vector<std::vector<db_cost*>*>();
			costForUnitLevel[i]->reserve(MAX_UNIT_LEVEL_NUMBER_DB);
			for (int j = 0; j < MAX_UNIT_LEVEL_NUMBER_DB; ++j) {
				costForUnitLevel[i]->push_back(new std::vector<db_cost*>);
			}
		}

		for (auto& unitUpgrade : unitUpgrades) {
			unitUpgrade = new std::vector<db_unit_upgrade*>();
		}

		for (auto& unitUpgradesCost : unitUpgradesCosts) {
			unitUpgradesCost = new std::vector<db_cost*>();
		}
	}

	~db_container() = default;
};
