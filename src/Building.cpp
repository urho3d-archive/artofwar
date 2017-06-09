#include "Building.h"
#include "ObjectEnums.h"
#include "db_strcut.h"


Building::Building(Vector3* _position, Urho3D::Node* _boxNode) : Entity(_position, _boxNode) {
	
}


Building::~Building() {
}


ObjectType Building::getType() {
	return BUILDING;
}

void Building::populate(db_building* dbBuilding) {
	minimalDistance = dbBuilding->minDist;
}

void Building::absorbAttack(double attackCoef) {
}
