#include "MainGrid.h"

MainGrid::MainGrid(short _resolution, double _size, bool _debugEnabled): Grid(_resolution, _size, _debugEnabled) {
	short posX = 0;
	short posZ = 0;

	int miniRes = resolution / 8;

	for (int i = 0; i < resolution * resolution; ++i) {
		buckets[i].upgrade();
		double cX = (posX + 0.5) * fieldSize - size / 2;
		double cZ = (posZ + 0.5) * fieldSize - size / 2;
		buckets[i].setCenter(cX, cZ);
		if (debugEnabled &&
			(cX > -miniRes && cX < miniRes) &&
			(cZ > -miniRes && cZ < miniRes)) {
			buckets[i].createBox(fieldSize);
		}
		++posZ;
		if (posZ >= resolution) {
			++posX;
			posZ = 0;
		}
	}
}

MainGrid::~MainGrid() {

}

bool MainGrid::validateAdd(Static* object) {
	IntVector2 size = object->getGridSize();
	Vector3* pos = object->getPosition();
	return validateAdd(size, pos);
}

bool MainGrid::validateAdd(const IntVector2& size, Vector3* pos) {
	short iX = getIndex(pos->x_);
	short iZ = getIndex(pos->z_);

	for (int i = iX; i < iX + size.x_; ++i) {
		for (int j = iZ; j < iZ + size.y_; ++j) {
			const int index = getIndex(i, j);
			if (!(inRange(index) &&
				buckets[index].getType() == ObjectType::UNIT)) {
				return false;
			}
		}
	}

	return true;
}

void MainGrid::addStatic(Static* object) {
	//TODO duzo poprawek trzeba, rozmair �rodek validacja, sprawdzenie przedziaa��w
	if (validateAdd(object)) {
		IntVector2 size = object->getGridSize();
		Vector3* pos = object->getPosition();
		short iX = getIndex(pos->x_);
		short iZ = getIndex(pos->z_);

		object->setBucket(getIndex(iX, iZ), 0);

		for (int i = iX; i < iX + size.x_; ++i) {
			for (int j = iZ; j < iZ + size.y_; ++j) {
				const int index = getIndex(i, j);
				buckets[index].setStatic(object);
			}
		}
	}
}

void MainGrid::removeStatic(Static* object) {
	int index = object->getBucketIndex(0);
	buckets[index].removeStatic();
}

Vector3* MainGrid::validatePosition(Vector3* position) {
	short posX = getIndex(position->x_);
	short posZ = getIndex(position->z_);
	const int index = getIndex(posX, posZ);
	if (buckets[index].getType() != ObjectType::UNIT) {

		Vector3* direction = buckets[index].getDirectrionFrom(position);

		direction->Normalize();
		return direction;
	}
	return nullptr;
}
