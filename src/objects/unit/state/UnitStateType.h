#pragma once
enum class UnitStateType :short
{
	GO = 0,//do celu
	STOP,
	CHARAGE,
	ATTACK,
	PATROL,
	DEAD,
	DEFEND,
	FOLLOW,//porusza sie
	COLLECT,
	MOVE,//TODO nie ma takiego orderu musi by� r�cznie dodany?
	DISPOSE
};
