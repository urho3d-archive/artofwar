#include "Formation.h"
#include "Game.h"
#include "commands/CommandList.h"
#include "commands/action/ActionCommand.h"
#include "commands/action/FormationAction.h"
#include "objects/unit/Unit.h"
#include "objects/unit/aim/FutureAim.h"
#include "simulation/env/Enviroment.h"
#include <algorithm>
#include <Urho3D/Math/Vector2.h>
#include <chrono>
#include <iostream>
#include <numeric>
#include <unordered_set>


Formation::Formation(short _id, std::vector<Physical*>* _units, FormationType _type, Vector2& _direction) : id(_id),
	type(_type), direction(_direction), state(FormationState::FORMING) {

	for (auto value : *_units) {
		auto unit = dynamic_cast<Unit*>(value);
		unit->clearAims();
		units.push_back(unit);
	}

	if (!units.empty()) {
		updateIds();
		updateCenter();
	} else {
		changeState(FormationState::EMPTY);
	}
	rechnessLevel = new char[units.size()];
	std::fill_n(rechnessLevel, units.size(), 0);
}

Formation::~Formation() = default;

Vector2 Formation::computeLocalCenter() {
	Vector2 localCenter = Vector2::ZERO;
	for (auto unit : units) {
		const auto pos = unit->getPosition();
		localCenter.x_ += pos->x_;
		localCenter.y_ += pos->z_;
	}
	localCenter /= units.size();
	return localCenter;
}

void Formation::setNewLeader(Vector2& localCenter) {
	int maxDist = 9999999;
	leader = nullptr;
	for (auto& unit : units) {
		const auto pos = unit->getPosition();
		const auto currentPos = Vector2(pos->x_, pos->z_);

		const auto sqDist = (currentPos - localCenter).LengthSquared();
		if (sqDist < maxDist) {
			leader = unit;
			maxDist = sqDist;
		}
	}
}

void Formation::electLeader() {
	Vector2 localCenter = computeLocalCenter();
	setNewLeader(localCenter);

	if (oldLeader != nullptr
		&& leader != oldLeader
		&& !futureOrders.empty()) {

		if (oldLeader->getFormation() == id) {
			oldLeader->clearAims();
		}
		changeState(FormationState::FORMING);
	}
	oldLeader = leader;
}

void Formation::updateIds() {
	if (changed) {
		updateSizes();
		electLeader();

		updateCenter();
		for (auto unit : units) {
			unit->setFormation(id);
			unit->setPositionInFormation(-1);
		}

		short cc = (short)((sideB - 1) / 2) * sideA + (short)(sideA / 2.0 + 0.5);
		if (cc >= units.size()) {
			cc = units.size() - 1;
		}
		leader->setPositionInFormation(cc);

		auto env = Game::getEnviroment();
		std::unordered_map<int, std::vector<short>> bucketToIds;
		for (int i = 0; i < units.size(); ++i) {
			if (leader->getPositionInFormation() == i) {
				continue;
			}
			auto pos = getPositionFor(i);
			int bucketForI = env->getIndex(pos);

			auto it = bucketToIds.find(bucketForI);
			if (it != bucketToIds.end()) {
				it->second.push_back(i);
			} else {
				auto inserted = bucketToIds.emplace(bucketForI, std::vector<short>());
				inserted.first->second.reserve(4);
				inserted.first->second.push_back(i);
			}
		}

		std::vector<short> tempVec(units.size());
		std::iota(tempVec.begin(), tempVec.end(), 0);

		short leaderID = leader->getPositionInFormation();
		tempVec[leaderID] = -1;
		short sizeToAssign = tempVec.size() - 1;
		for (auto unit : units) {
			if (leader == unit) {
				continue;
			}
			int bucketId = unit->getBucketIndex(-1);
			const auto pos = unit->getPosition();
			const auto currentPos = Vector2(pos->x_, pos->z_);


			auto it = bucketToIds.find(bucketId);
			if (it != bucketToIds.end()) {
				short bestId = -1;
				float bestSize = 99999;
				for (int i = 0; i < it->second.size(); ++i) {
					auto id = it->second[i];
					if (tempVec[id] != -1) {
						const auto sqDist = (currentPos - getPositionFor(id)).LengthSquared();
						if (sqDist < bestSize) {
							bestSize = sqDist;
							bestId = i;
						}
					}
				}

				if (bestId >= 0) {
					--sizeToAssign;
					tempVec[it->second[bestId]] = -1;
					unit->setPositionInFormation(it->second[bestId]);
					it->second.erase(it->second.begin() + bestId);
				}
			}
		}
		std::vector<short> output;
		output.reserve(sizeToAssign);
		std::copy_if(tempVec.begin(), tempVec.end(), std::back_inserter(output), [](const short i) { return i >= 0; });

		int i = 0;
		for (auto unit : units) {
			if (unit->getPositionInFormation() == -1) {
				unit->setPositionInFormation(output[i]);
				++i;
			}
		}
		changed = false;
	}
}

void Formation::updateSizes() {
	sideA = sqrt(units.size()) + 0.5;
	sideB = units.size() / sideA;
}

void Formation::calculateNotWellFormed() {
	notWellFormed = 0;
	notWellFormedExact = 0;
	for (auto unit : units) {
		const auto pos = unit->getPosition();

		const auto currentPos = Vector2(pos->x_, pos->z_);
		const auto desiredPos = getPositionFor(unit->getPositionInFormation());
		const auto sqDist = (currentPos - desiredPos).LengthSquared();

		if (sqDist < 0.5) {
			rechnessLevel[unit->getPositionInFormation()] = 0;
		} else if (sqDist < 2 * 2) {
			notWellFormedExact += 1;
			rechnessLevel[unit->getPositionInFormation()] = 1;
		} else {
			notWellFormed += 1;
			notWellFormedExact += 1;
			rechnessLevel[unit->getPositionInFormation()] = 2;
		}
	}
	notWellFormed /= units.size();
	notWellFormedExact /= units.size();
}

void Formation::innerUpdate() {
	updateUnits();

	if (!units.empty()) {
		updateIds();
		updateCenter();
		calculateNotWellFormed();
	} else {
		changeState(FormationState::EMPTY);
	}
}

void Formation::update() {
	switch (state) {
	case FormationState::FORMING:
		innerUpdate();
		if (notWellFormed < theresholedMin) {
			changeState(FormationState::MOVING);
			if (!futureOrders.empty()) {
				const auto& futureOrder = futureOrders[0];
				Game::getActionCommandList()->add(new FormationAction(this,
				                                                             futureOrder.action,
				                                                             futureOrder.physical,
				                                                             new Vector2(futureOrder.vector)
				                                                            ));
				futureOrders.erase(futureOrders.begin()); //TODO to zachowa�
			}
		}
		break;
	case FormationState::MOVING:
		innerUpdate();
		if (notWellFormed > theresholedMax) {
			changeState(FormationState::FORMING);
		} else if (notWellFormedExact < theresholedMin
			&& !leader->hasAim()) {
			if (futureOrders.empty()) {
				changeState(FormationState::REACHED);
			} else {
				changeState(FormationState::FORMING);
			}
		}
		break;
	case FormationState::REACHED:
		for (auto unit : units) {
			unit->resetFormation();
		}
		units.clear();
		changeState(FormationState::EMPTY);
		break;
	}
}

void Formation::changeState(FormationState newState) {
	state = newState;
}

Vector2 Formation::getPositionFor(short id) {
	const int columnThis = id % sideA;
	const int rowThis = id / sideA;
	const short leaderID = leader->getPositionInFormation();

	const int columnLeader = leaderID % sideA;
	const int rowLeader = leaderID / sideA;

	const int column = columnThis - columnLeader;
	const int row = rowThis - rowLeader;

	return center - Vector2(column * sparsity, row * sparsity);
}

float Formation::getPriority(int id) const {
	return rechnessLevel[id];
}

std::optional<Physical*> Formation::getLeader() {
	if (state != FormationState::REACHED && units.size() > leader->getPositionInFormation()) {
		return leader;
	}
	return {};
}

void Formation::addAim(const Vector2& _vector, const Physical* _physical,
                       UnitOrder _action, bool append) {
	if (!append) {
		futureOrders.clear();
	}
	futureOrders.emplace_back(_vector, _physical, _action);
}

size_t Formation::getSize() {
	return units.size();
}

void Formation::semiReset() {
	notWellFormed = 1;
	notWellFormedExact = 1;
	changed = true;
	futureOrders.clear();
	changeState(FormationState::FORMING);
}

void Formation::updateUnits() {
	units.erase(
	            std::remove_if(
	                           units.begin(), units.end(),
	                           [this](Unit* unit)
	                           {
		                           bool ifErase = !unit->isAlive() || unit->getFormation() != id;
		                           if (ifErase) {
			                           if (unit->getFormation() == id) {
				                           unit->resetFormation();
			                           }
			                           changed = true;
		                           }
		                           return ifErase;
	                           }),
	            units.end());
}

void Formation::updateCenter() {
	Vector3* leaderPos = leader->getPosition();
	center = Vector2(leaderPos->x_, leaderPos->z_);
}
