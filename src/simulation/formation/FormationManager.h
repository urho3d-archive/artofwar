#pragma once
#include "Formation.h"
#include <optional>
#include <vector>


class FormationManager
{
public:
	FormationManager();
	~FormationManager();
	void createFormation(std::vector<Physical*>* _units, FormationType _type = FormationType::SQUERE);
	void update();
	std::optional<Vector3> getPositionFor(Unit* unit);
private:
	std::vector<Formation*> formations;
	int currentlyFree = 0;
	
};