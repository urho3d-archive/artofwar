#include "CameraManager.h"
#include "FreeCameraBehave.h"
#include "Game.h"
#include "RtsCameraBehave.h"
#include "TopCameraBehave.h"
#include "simulation/env/Enviroment.h"
#include <Urho3D/Graphics/Graphics.h>


CameraManager::CameraManager() {
	cameraBehaves.push_back(new FreeCameraBehave());
	cameraBehaves.push_back(new RtsCameraBehave());
	cameraBehaves.push_back(new TopCameraBehave());

	activeBehave = cameraBehaves.at(1);
	float border = 256.f;

	const int width = Game::getGraphics()->GetWidth();
	const int height = Game::getGraphics()->GetHeight();
	widthEdge = width / border;
	heightEdge = height / border;

	widthEdgeMax = width - widthEdge;
	heightEdgeMax = height - heightEdge;
}

CameraManager::~CameraManager() {
	clear_vector(cameraBehaves);
}

void CameraManager::setCameraBehave(int _type) {
	activeBehave = cameraBehaves.at(_type);
}

Urho3D::Camera* CameraManager::getComponent() const {
	return activeBehave->getComponent();
}

void CameraManager::createCameraKeys(Input* input, bool cameraKeys[4], const IntVector2& cursorPos) const {
	cameraKeys[0] = input->GetKeyDown(KEY_W);
	cameraKeys[1] = input->GetKeyDown(KEY_S);
	cameraKeys[2] = input->GetKeyDown(KEY_A);
	cameraKeys[3] = input->GetKeyDown(KEY_D);

	if (cursorPos.x_ < widthEdge) {
		cameraKeys[2] = true;
	} else if (cursorPos.x_ > widthEdgeMax) {
		cameraKeys[3] = true;
	}
	if (cursorPos.y_ < heightEdge) {
		cameraKeys[0] = true;
	} else if (cursorPos.y_ > heightEdgeMax) {
		cameraKeys[1] = true;
	}
}

void CameraManager::translate(const IntVector2& cursorPos, Input* input, float timeStep) const {
	bool cameraKeys[4];
	createCameraKeys(input, cameraKeys, cursorPos);
	const int wheel = input->GetMouseMoveWheel();
	const Vector3 pos = activeBehave->getPosition();
	const float min = Game::getEnviroment()->getGroundHeightAt(pos.x_, pos.z_);
	activeBehave->translate(cameraKeys, wheel, timeStep * MOVE_SPEED, min);
}

String* CameraManager::getInfo() const {
	return activeBehave->getInfo();
}

Urho3D::MouseMode CameraManager::getMouseMode() const {
	return activeBehave->getMouseMode();
}

void CameraManager::rotate(const IntVector2& mouse_move) const {
	activeBehave->rotate(mouse_move, MOUSE_SENSITIVITY);
}

void CameraManager::changePosition(float x, float y) const {
	activeBehave->changePosition(x, y);
}
