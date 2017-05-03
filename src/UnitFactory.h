#pragma once
#include "Unit.h"
#include <vector>
#include "EntityFactory.h"
#include "defines.h"
#include "SpacingType.h"
#include "UnitType.h"
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Graphics/Model.h>
#include "LinkComponent.h"
#include "Game.h"

class UnitFactory :public EntityFactory {
public:
	UnitFactory();
	~UnitFactory();

	double getSpecSize(SpacingType spacing);
	String getModelName(UnitType unitType);
	std::vector<Unit*>* createUnits(unsigned int number, UnitType unitType, Vector3 *center, SpacingType spacingType);

	//int size = UNITS_NUMBER;
	//double space = INIT_SPACE;
};

 