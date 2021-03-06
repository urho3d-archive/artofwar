#pragma once
#include <Urho3D/Math/Vector2.h>


class Unit;

class Aim
{
public:

	virtual ~Aim() = default;
	virtual Urho3D::Vector2 getDirection(Unit* unit) = 0;
	virtual bool ifReach(Unit* unit) = 0;
	virtual bool expired() = 0;
};
