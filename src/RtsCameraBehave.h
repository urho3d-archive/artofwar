#pragma once
#include "CameraBehave.h"
using namespace Urho3D;

class RtsCameraBehave : public CameraBehave {
public:
	RtsCameraBehave(Urho3D::Context* context);
	~RtsCameraBehave();
	virtual void translate(bool cameraKeys[], int wheel, float timeStep);
	virtual void rotate(const IntVector2& mouseMove, const double mouse_sensitivity);

	virtual void setRotation(const Urho3D::Quaternion& rotation);
	virtual Urho3D::String getInfo();
	virtual MouseMode getMouseMode();
private:
	float maxY=100, minY=5;
};

