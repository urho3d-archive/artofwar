#pragma once
#include <vector>
#include "Unit.h"

class Bucket {
public:
	Bucket();
	~Bucket();

	std::vector<Physical*> *getContent();
	void add(Physical* entity);
	void remove(Physical* entity);
	void clearContent();
private:
	std::vector <Physical*>* content;
};

