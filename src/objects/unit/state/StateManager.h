#pragma once
#include "State.h"
#include "database/db_strcut.h"
#include "objects/unit/ActionParameter.h"

class StateManager
{
public:

	void changeState(Unit* unit, UnitStateType stateTo);
	bool validateState(int id, UnitStateType stateTo);
	void changeState(Unit* unit, UnitStateType stateTo, ActionParameter& actionParameter);
	bool checkChangeState(Unit* unit, UnitStateType stateTo);
	void execute(Unit* unit);
	static StateManager* get();

	static void init();
	static void dispose();
private:
	StateManager();
	~StateManager();
	State* states[STATE_SIZE]{};
	bool ordersToUnit[UNITS_NUMBER_DB][STATE_SIZE];

	static StateManager* instance;
};
