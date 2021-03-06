#pragma once
#include "camera/CameraBehave.h"
using namespace Urho3D;

class FreeCameraBehave : public CameraBehave
{
public:
	FreeCameraBehave();
	virtual ~FreeCameraBehave();

	void translate(bool cameraKeys[], int wheel, float timeStep, float min) override;
	void rotate(const IntVector2& mouseMove, double mouse_sensitivity) override;
	void setRotation(const Urho3D::Quaternion& rotation) override;
	Urho3D::String* getInfo() override;
	MouseMode getMouseMode() override;
private:
	double yaw = 0.0;
	double pitch = 0.0;
};
