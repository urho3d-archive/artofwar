#include "UnitFactory.h"


UnitFactory::UnitFactory(): EntityFactory() {
	units = new std::vector<Unit *>();
	units->reserve(DEFAULT_VECTOR_SIZE);
	states = new StateManager();
	Unit::setStates(states);
}

UnitFactory::~UnitFactory() {
	units->clear();
	delete units;
	delete states;
}

std::vector<Unit*>* UnitFactory::create(unsigned number, int id, Vector3* center, int player) {
	units->clear();

	double space = getSpecSize(CONSTANT);

	int y = 0;
	int xMax = number / sqrt(number);
	double sideSize = xMax * space / 2;

	while (units->size() < number) {
		for (int x = 0; x < xMax; ++x) {
			Vector3* position = new Vector3(x * space + center->x_ - sideSize, 0 + center->y_,
			                                y * space + center->z_ - sideSize);
			Unit* newUnit = new Unit(position, id, player);

			units->push_back(newUnit);
			if (units->size() >= number) { break; }
		}
		++y;
	}
	return units;
}

std::vector<Unit*>* UnitFactory::load(dbload_unit* unit) {
	units->clear();


	Vector3* position = new Vector3(unit->pos_x, 0, unit->pos_z);
	Unit* newUnit = new Unit(position, unit->id_db, unit->player);
	newUnit->load(unit);

	units->push_back(newUnit);

	return units;
}
