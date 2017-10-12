#pragma once
#include <vector>
#include "CameraEnums.h"
#include "CameraBehave.h"
#include "FreeCameraBehave.h"
#include "RtsCameraBehave.h"

using namespace Urho3D;

class CameraManager {
public:
	CameraManager();
	~CameraManager();
	void setCameraBehave(int _type);
	Camera *getComponent();
	void translate(const IntVector2& cursorPos, Input* input, float timeStep);
	String *getInfo();
	MouseMode getMouseMode();
	void rotate(const IntVector2& mouse_move);
private:
	void createCameraKeys(Input* input, bool cameraKeys[4], const IntVector2& cursorPos);

	const double MOUSE_SENSITIVITY = 0.1f;
	std::vector<CameraBehave*> cameraBehaves;
	CameraBehave* activeBehave;
	float MOVE_SPEED = 20;

	int widthEdge;
	int heightEdge;

	int widthEdgeMax;
	int heightEdgeMax;
};

