#pragma once
#define STATE_SIZE 12

enum class UnitStateType : char
{
	GO = 0,
	//do celu
	STOP,
	CHARAGE,
	ATTACK,
	PATROL,
	DEAD,
	DEFEND,
	FOLLOW,
	//porusza sie
	COLLECT,
	MOVE,
	//TODO nie ma takiego orderu musi by� r�cznie dodany?
	DISPOSE,
	SHOT
};
