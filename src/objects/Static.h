#pragma once
#include "Physical.h"
#include "SurroundState.h"

struct SurroundCell
{
	SurroundCell(int index, SurroundState state, char size)
		: index(index),
		state(state),
		size(size) {
	}

	int index;
	SurroundState state;
	char size;
};


class Static : public Physical
{
public:
	Static(Vector3* _position, ObjectType _type);
	virtual ~Static();

	void setMainCell(const IntVector2& _mainCell);
	static std::string getColumns();

	Vector2 getClosestCellPos(Vector3* pos) const;
	IntVector2& getBucketPosition() { return mainCell; }
	IntVector2& getGridSize() { return gridSize; }
	std::vector<int>& getOcupiedCells() { return ocupiedCells; }
	std::vector<SurroundCell>& getSurroundCells() { return surroundCells; }

	Vector2 getPosToFollow(Vector3* center) const override;
	std::string getValues(int precision) override;
protected:
	void populate(const IntVector2& size);

	std::vector<int> ocupiedCells;
	std::vector<SurroundCell> surroundCells;

	IntVector2 gridSize;
	IntVector2 mainCell;
};
