#pragma once
#include "State.h"

class StopState :public State
{
public:
	StopState();
	~StopState();
	void onStart(Unit* unit) override;
	void onEnd(Unit* unit) override;
	void execute(Unit* unit) override;
};
