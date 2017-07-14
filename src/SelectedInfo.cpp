#include "SelectedInfo.h"

void SelectedInfo::initLines() {
	for (int i = 0; i < SELECTED_INFO_SIZE; ++i) {
		lines[i] = nullptr;
	}
}

void SelectedInfo::clearLines() {
	for (int i = 0; i < SELECTED_INFO_SIZE; ++i) {
		if (lines[i] != nullptr) {
			lines[i] = nullptr;
		}
	}
}

SelectedInfo::SelectedInfo() {
	message = new Urho3D::String();
	allNumber = 0;

	lines = new Urho3D::String*[SELECTED_INFO_SIZE];
	initLines();
}


SelectedInfo::~SelectedInfo() {
}

Urho3D::String* SelectedInfo::getMessage() {
	delete message;
	message = new Urho3D::String("Zaznaczeni: " + Urho3D::String(allNumber));
	changed = false;
	return message;
}

Urho3D::String** SelectedInfo::getLines() {
	clearLines();
	for (int i = 0; i < SELECTED_INFO_SIZE; ++i) {
		if (number[i] > 0) {
			lines[i] = new Urho3D::String(Urho3D::String(number[i]));
		} else {
			lines[i] = nullptr;
		}

	}
	return lines;
}

bool SelectedInfo::hasChanged() {
	return changed;
}

void SelectedInfo::setAllNumber(int allNumber) {
	this->allNumber = allNumber;
	changed = true;
}

void SelectedInfo::setSelectedType(ObjectType selectedType) {
	this->selectedType = selectedType;
	changed = true;
}

ObjectType SelectedInfo::getSelectedType() {
	return selectedType;
}

void SelectedInfo::setNumberAt(int type, int value) {
	number[type] = value;
	changed = true;
}

void SelectedInfo::incNumberAt(int type) {
	number[type]++;
	changed = true;
}

void SelectedInfo::reset() {
	changed = true;
	this->allNumber = 0;
	for (int i = 0; i < SELECTED_INFO_SIZE; ++i) {
		number[i] = 0;
	}
}
