#pragma once
#include "Aim.h"
#include <Urho3D/Scene/Node.h>

class ChargeAim: public Aim
{
public:
	ChargeAim(Urho3D::Vector3* _direction);
	~ChargeAim();
	Urho3D::Vector2 getDirection(Unit* unit) override;
	bool ifReach(Unit* unit) override;
	bool expired() override;
private:
	Urho3D::Vector3 direction;
	double distance;
};

