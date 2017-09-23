#pragma once
#include <Urho3D/Container/Str.h>
#include "database/db_strcut.h"

class Resources
{
public:
	Resources();
	Resources(double valueForAll);
	~Resources();
	bool reduce(std::vector<db_cost*>* costs);
	void add(int id, double value);
	bool hasChanged();
	short getSize();
	double *getValues();
	Urho3D::String** getNames();
	void hasBeedUpdatedDrawn();
private:
	bool changed;
	void revert(int end, std::vector<db_cost*>* costs);
	Urho3D::String* names[RESOURCE_NUMBER_DB];
	double values[RESOURCE_NUMBER_DB];
	int size;
};
