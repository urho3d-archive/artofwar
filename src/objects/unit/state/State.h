#pragma once
#include "UnitState.h"
#include "objects/unit/ActionParameter.h"


class Unit;

class State
{
public:
	State();
	virtual ~State();
	virtual void onStart(Unit* unit, ActionParameter& parameter) =0;
	virtual void onEnd(Unit* unit);
	virtual void execute(Unit* unit);

	bool validateTransition(UnitState stateTo);
protected:
	bool nextStates[STATE_SIZE];
};
