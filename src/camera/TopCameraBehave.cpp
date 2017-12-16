#include "TopCameraBehave.h"
#include "Game.h"


TopCameraBehave::TopCameraBehave():CameraBehave(20, "TopCam") {
	double diff = sqrt(50.0f - minY) + 1;
	cameraNode->SetDirection(Urho3D::Vector3::DOWN * diff);
	camera->SetOrthographic(true);
}


TopCameraBehave::~TopCameraBehave() {
}

void TopCameraBehave::translate(bool cameraKeys[], int wheel, float timeStep, float min) {
	double diff = sqrt((orthoSize - minY) / 10) + 1;

	if (cameraKeys[0]) {
		cameraNode->Translate(diff * Urho3D::Vector3::FORWARD * timeStep, Urho3D::TS_WORLD);
		changed = true;
	}
	if (cameraKeys[1]) {
		cameraNode->Translate(diff * Urho3D::Vector3::BACK * timeStep, Urho3D::TS_WORLD);
		changed = true;
	}
	if (cameraKeys[2]) {
		cameraNode->Translate(diff * Urho3D::Vector3::LEFT * timeStep, Urho3D::TS_WORLD);
		changed = true;
	}
	if (cameraKeys[3]) {
		cameraNode->Translate(diff * Urho3D::Vector3::RIGHT * timeStep, Urho3D::TS_WORLD);
		changed = true;
	}
	if (wheel != 0) {
		orthoSize -= wheel * (sqrt(orthoSize - minY) + 1) * 1.5;

		if (orthoSize < minY) {
			orthoSize = minY;
		} else if (orthoSize > maxY) {
			orthoSize = maxY;
		}
		camera->SetOrthoSize(orthoSize);
		
		changed = true;
	}
}

void TopCameraBehave::rotate(const IntVector2& mouseMove, const double mouse_sensitivity) {

}

void TopCameraBehave::setRotation(const Urho3D::Quaternion& rotation) {

}

Urho3D::String* TopCameraBehave::getInfo() {
	if (changed) {
		(*info) = name + " \t" + cameraNode->GetPosition().ToString() + "\n" + cameraNode->GetRotation().ToString();
		changed = false;
	}
	return info;
}

MouseMode TopCameraBehave::getMouseMode() {
	return MM_RELATIVE;
}
