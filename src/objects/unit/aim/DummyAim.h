#pragma once
#include "Aim.h"

class DummyAim : public Aim
{
public:
	DummyAim() = default;
	~DummyAim() override = default;

	Urho3D::Vector2 getDirection(Unit* unit) override {
		return Urho3D::Vector2();
	}

	bool ifReach(Unit* unit) override {
		return true;
	}

	bool expired() override {
		return false;
	}
};
