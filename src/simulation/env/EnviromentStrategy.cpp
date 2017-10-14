#include "EnviromentStrategy.h"


EnviromentStrategy::EnviromentStrategy() {
	allUnitGrid = new BucketGrid(BUCKET_GRID_RESOLUTION, BUCKET_GRID_SIZE, true);
	for (int i = 0; i < MAX_PLAYERS; ++i) {
		teamUnitGrid[i] = new BucketGrid(BUCKET_GRID_RESOLUTION_ENEMY, BUCKET_GRID_SIZE_ENEMY);
	}

	obstacleGrid = new BucketGrid(BUCKET_GRID_RESOLUTION_BUILD, BUCKET_GRID_SIZE_BUILD);
	resourceGrid = new BucketGrid(BUCKET_GRID_RESOLUTION_RESOURCE, BUCKET_GRID_SIZE_RESOURCE);

}


EnviromentStrategy::~EnviromentStrategy() {
	delete obstacleGrid;
	delete resourceGrid;

	delete allUnitGrid;
	delete[] teamUnitGrid;
}

float EnviromentStrategy::getSqDistance(Vector3* unitPosition, Vector3* otherPosition) {
	return ((*otherPosition) - (*unitPosition)).LengthSquared();
}

std::vector<Unit*>* EnviromentStrategy::getNeighbours(Unit* unit, double radius) {
	return getNeighbours(unit, allUnitGrid, radius);
}

std::vector<Unit*>* EnviromentStrategy::getNeighboursFromTeam(Unit* unit, double radius, int team,
                                                              OperatorType operatorType) {
	switch (operatorType) {
	case EQUAL:
		return getNeighbours(unit, teamUnitGrid[team], radius);
	case NOT_EQUAL:
		{
		std::vector<Unit*>* neight = new std::vector<Unit*>();
		for (int i = 0; i < MAX_PLAYERS; ++i) {
			if (team != i) {
				std::vector<Unit*>* neight1 = getNeighbours(unit, teamUnitGrid[i], radius);
				neight->insert(neight->end(), neight1->begin(), neight1->end());
				delete neight1;
			}
		}
		return neight;
		}
	default:
		return new std::vector<Unit*>();
	}
}

//std::vector<Physical*>* EnviromentStrategy::getBuildings(Unit* unit, double radius) {
//	return getNeighbours(unit, obstacleGrid, radius);
//}

std::vector<Unit *>* EnviromentStrategy::getNeighbours(Unit* unit, BucketGrid* bucketGrid, double radius) {
	std::vector<Unit*>* neights = new std::vector<Unit *>();
	neights->reserve(DEFAULT_VECTOR_SIZE);//TODO sparametryzowac

	double sqSeparationDistance = radius * radius;
	Vector3* unitPosition = unit->getPosition();
	BucketIterator* bucketIterator = bucketGrid->getArrayNeight(unit, radius, 0);
	while (Unit* neight = bucketIterator->next()) {
		if (unit == neight) { continue; }

		double sqDistance = getSqDistance(unitPosition, neight->getPosition());

		if (sqDistance < sqSeparationDistance) {
			neights->push_back(neight);
		}
	}

	return neights;
}

//std::vector<Physical*>* EnviromentStrategy::getResources(Unit* unit, double radius) {
//	return getNeighbours(unit, resourceGrid, radius);
//}

void EnviromentStrategy::update(std::vector<Unit*>* units) {
	for (auto unit : (*units)) {
		allUnitGrid->updateGrid(unit, 0);
		teamUnitGrid[unit->getTeam()]->updateGrid(unit, 1);
	}
}

void EnviromentStrategy::update(std::vector<Building*>* buildings) {
	for (auto building : (*buildings)) {
		allUnitGrid->addStatic(building);
	}
}

void EnviromentStrategy::update(std::vector<ResourceEntity*>* resources) {
	for (auto resource : (*resources)) {
		allUnitGrid->addStatic(resource);
	}
}

//Vector2& EnviromentStrategy::getRepulsiveAt(Vector3* position) {
//	return gradient->getValueAt(position->x_, position->z_);
//}

Vector3* EnviromentStrategy::validatePosition(Vector3* position) {
	return allUnitGrid->validatePosition(position);
}

std::vector<Physical*>* EnviromentStrategy::getNeighbours(std::pair<Vector3*, Vector3*>* pair) {
	return allUnitGrid->getArrayNeight(pair);
}

std::vector<Physical*>* EnviromentStrategy::getBuildings(std::pair<Vector3*, Vector3*>* pair) {
	return obstacleGrid->getArrayNeight(pair);
}

double EnviromentStrategy::getGroundHeightAt(double x, double z) {
	return 0.0;
}
