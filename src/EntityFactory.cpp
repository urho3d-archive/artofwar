#include "EntityFactory.h"

EntityFactory::EntityFactory() {
}

EntityFactory::~EntityFactory() {
}

double EntityFactory::getSpecSize(SpacingType spacing) {
	switch (spacing) {
	case CONSTANT:
	case RANDOM:
		return INIT_SPACE;
	}
}
