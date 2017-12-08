#pragma once
#include <vector>
#include "utils.h"

struct dbload_config
{
	int precision;
	int map;
};

struct dbload_physical
{
	int id_db;
	bool alive;
	float hp_coef;
	int player;


	dbload_physical(int idDb, bool alive, float hpCoef, int player)
		: id_db(idDb),
		alive(alive),
		hp_coef(hpCoef),
		player(player) {
	}
};

struct dbload_static : dbload_physical
{
	int buc_x;
	int buc_y;


	dbload_static(int idDb, bool alive, float hpCoef, int player, int bucX, int bucY)
		: dbload_physical(idDb, alive, hpCoef, player),
		buc_x(bucX),
		buc_y(bucY) {
	}
};

struct dbload_unit : dbload_physical
{
	float pos_x;
	float pos_z;
	int state;
	float vel_x;
	float vel_z;
	int aim_i;


	dbload_unit(int idDb, bool alive, float hpCoef, int player, float posX, float posZ, int state, float velX, float velZ,
	            int aimI)
		: dbload_physical(idDb, alive, hpCoef, player),
		pos_x(posX),
		pos_z(posZ),
		state(state),
		vel_x(velX),
		vel_z(velZ),
		aim_i(aimI) {
	}
};

struct dbload_building : dbload_static
{
	float target_x;
	float target_z;


	dbload_building(int idDb, bool alive, float hpCoef, int player, int bucX, int bucY, float targetX, float targetZ)
		: dbload_static(idDb, alive, hpCoef, player, bucX, bucY),
		target_x(targetX),
		target_z(targetZ) {
	}
};

struct dbload_resource : dbload_static
{
	float amount;

	dbload_resource(int idDb, bool alive, float hpCoef, int player, int bucX, int bucY, float amount)
		: dbload_static(idDb, alive, hpCoef, player, bucX, bucY),
		amount(amount) {
	}
};

struct dbload_load
{
	dbload_load() {
		precision = 1;
		config = new dbload_config();
		units = new std::vector<dbload_unit*>();
		buildings = new std::vector<dbload_building*>();
		resource_entities = new std::vector<dbload_resource*>();
	}

	~dbload_load() {
		delete config;
		clear_and_delete_vector(units);
		clear_and_delete_vector(buildings);
		clear_and_delete_vector(resource_entities);
	}

	int precision;
	dbload_config* config;

	std::vector<dbload_unit*>* units;
	std::vector<dbload_building*>* buildings;
	std::vector<dbload_resource*>* resource_entities;
};
