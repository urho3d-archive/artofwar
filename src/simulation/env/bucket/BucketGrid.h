#pragma once
#include "Bucket.h"
#include <unordered_map>

#define MAX_SEP_DIST 20
#define RES_SEP_DIST 200
class BucketIterator;

class BucketGrid
{
public:
	BucketGrid(short _resolution, double _size);
	~BucketGrid();

	std::vector<Physical *>* getContentAt(short x, short z);
	void updateGrid(Physical* entity, short team);
	std::vector<std::pair<short, short>*>* getEnvIndexsFromCache(double getMaxSeparationDistance);

	BucketIterator* getArrayNeight(Unit* entity, double radius, short thread);
	std::vector<Physical *>* getArrayNeight(std::pair<Vector3*, Vector3*>* pair);
private:
	BucketIterator** iterators;
	bool fieldInCircle(int i, int j, double radius);
	std::vector<std::pair<short, short>*>* getEnvIndexs(double radius);
	int getIntegerPos(double value);
	bool isInSide(int _posX, int _posZ) const;
	void addAt(short x, short z, Physical* entity);
	void removeAt(short x, short z, Physical* entity);

	Bucket** buckets;
	short resolution;
	short halfResolution;
	double size;
	double fieldSize;

	double diff = ((double)MAX_SEP_DIST) / RES_SEP_DIST;

	std::vector<std::pair<short, short>*>** levelsCache;
	std::vector<Physical*>* empty;
};
