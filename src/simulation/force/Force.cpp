#include "Force.h"
#include "Game.h"

Force::Force() = default;


Force::~Force() = default;

void Force::separationObstacle(Vector2& newForce, Unit* unit, const Vector2& repulse) {
	if (repulse == Vector2::ZERO) {
		return;
	}
	Vector2 force(repulse.x_, repulse.y_);
	const auto distance = force.Length();
	const auto minimalDistance = unit->getMinimalDistance() * 3;
	const auto coef = calculateCoef(distance, minimalDistance);

	force *= coef / distance;
	force *= boostCoef * sepCoef;

	forceStats.addSepObst(force);

	newForce += force;
}

void Force::separationUnits(Vector2& newForce, Unit* unit, std::vector<Unit*>* units) {
	if (units->empty()) {
		return;
	}
	Vector2 force;
	int isLeaderFor = Game::get()->getFormationManager()->isLeaderFor(unit);

	for (auto neight : *units) {
		float sqSepDist = unit->getMaxSeparationDistance() + neight->getMinimalDistance();
		sqSepDist *= sqSepDist;

		Vector2 diff(
		             unit->getPosition()->x_ - neight->getPosition()->x_,
		             unit->getPosition()->z_ - neight->getPosition()->z_
		            );
		const float sqDistance = diff.LengthSquared();
		if (sqDistance > sqSepDist) { continue; }
		if (isLeaderFor != -1 && isLeaderFor == neight->getFormation()) { continue; }
		if (sqDistance == 0) {
			force.x_ = static_cast<float>(rand()) / RAND_MAX - 0.5;
			force.y_ = static_cast<float>(rand()) / RAND_MAX - 0.5;
			force *= boostCoef * sepCoef;
			newForce += force;
			return;
		}
		const float distance = sqrt(sqDistance);

		const float minimalDistance = unit->getMinimalDistance() + neight->getMinimalDistance();
		const float coef = calculateCoef(distance, minimalDistance);

		diff *= coef / distance;
		force += diff;
	}

	force *= boostCoef * sepCoef;

	forceStats.addSepUnit(force);

	newForce += force;
}

void Force::destination(Vector2& newForce, Unit* unit) {
	auto force = unit->getDestination(boostCoef, aimCoef);

	forceStats.addDest(force);

	newForce += force;
}

void Force::formation(Vector2& newForce, Unit* unit) {
	auto opt = Game::get()->getFormationManager()->getPositionFor(unit);
	if (opt.has_value()) {
		const float priority = Game::get()->getFormationManager()->getPriority(unit);
		auto force = Vector2(
		                     opt.value().x_ - unit->getPosition()->x_,
		                     opt.value().y_ - unit->getPosition()->z_
		                    );

		force *= formationCoef * boostCoef * priority;

		forceStats.addForm(force);

		newForce += force;
	}
}

void Force::escapeFromInvalidPosition(Vector2& newForce, Vector2* dir) {
	if (dir) {
		auto force = *dir * escapeCoef * boostCoef;

		forceStats.addEscp(force);

		newForce += force;
		delete dir;
	}
}

float* Force::stats() {
	return forceStats.result();
}

void Force::changeCoef(int i, int wheel) {
	switch (i) {
	case 0: sepCoef += wheel * 0.2;
		std::cout << "Sep" << sepCoef << std::endl;
		break;
	case 1: aimCoef += wheel * 0.2;
		std::cout << "Aim " << aimCoef << std::endl;
		break;
	case 2: formationCoef += wheel * 0.2;
		std::cout << "Form " << formationCoef << std::endl;
		break;
	case 3: escapeCoef += wheel * 0.2;
		std::cout << "Esc " << escapeCoef << std::endl;
		break;
	default:
		break;
	}
}

float Force::calculateCoef(const float distance, const float minDist) const {
	float parameter = distance - minDist / 2;
	if (parameter <= 0.00005) {
		parameter = 0.00005;
	}
	return 1 / parameter;
}

//float Force::calculateCoef(const float distance, const float minDist) const {
//	return exp(minDist / (distance + 0.05)) - 1;
//}

//float Force::calculateCoef(const float distance, const float minDist) const {
//	double parameter = distance - minDist/2;
//	if (parameter <= 0.05) {
//		parameter = 0.05;
//	}
//	return exp(1 / parameter) - 1;
//}
