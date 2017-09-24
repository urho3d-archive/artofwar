#include "SelectedInfoType.h"


SelectedInfoType::SelectedInfoType() {
	data = new std::vector<Physical*>();
	data->reserve(1000);
	id = -1;
}


SelectedInfoType::~SelectedInfoType() {
}

void SelectedInfoType::clear() {
	data->clear();
	id = -1;
}

void SelectedInfoType::add(Physical* physical) {
	data->push_back(physical);
	id = physical->getID();
}

std::vector<Physical*>* SelectedInfoType::getData() {
	return data;
}

int SelectedInfoType::getId() {
	return id;
}
