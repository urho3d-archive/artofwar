#include "FormationManager.h"
#include "objects/unit/Unit.h"
#include "utils.h"
#include <optional>


FormationManager::FormationManager() = default;


FormationManager::~FormationManager() {
	clear_vector(formations);
}

void FormationManager::createFormation(std::vector<Physical*>* _units, FormationType _type) {
	if (_units->empty()) { return; }
	if (_type == FormationType::NONE) {
		for (auto unit : *_units) {
			static_cast<Unit*>(unit)->setFormation(-1);
			static_cast<Unit*>(unit)->setPositionInFormation(-1);
		}
	} else {
		for (; currentlyFree < formations.size(); ++currentlyFree) {
			if (formations[currentlyFree] == nullptr) {
				break;
			}
		}

		if (currentlyFree == formations.size()) {
			formations.push_back(nullptr);
		}
		formations[currentlyFree] = new Formation(currentlyFree, _units, _type, Vector2(1, 1));
		currentlyFree++;
	}
}

void FormationManager::update() {
	for (int i = 0; i < formations.size(); ++i) {
		if (formations[i]) {
			formations[i]->update();
			if (formations[i]->getState() == FormationState::EMPTY) {
				delete formations[i];
				formations[i] = nullptr;
				if (i < currentlyFree) {
					currentlyFree = i;
				}
			}
		}
	}
}

float FormationManager::getPriority(Unit* unit) {
	const short formation = unit->getFormation();
	if (formation >= 0) {
		return formations[formation]->getPriority(unit->getPositionInFormation());
	}
	return 0.0;
}

std::optional<Vector2> FormationManager::getPositionFor(Unit* unit) {
	const short formation = unit->getFormation();
	if (formation >= 0) {
		return formations[formation]->getPositionFor(unit->getPositionInFormation());
	}
	return std::nullopt;
}
