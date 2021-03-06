#include "Unit.h"
#include "Game.h"
#include "MathUtils.h"
#include "ObjectEnums.h"
#include "UnitOrder.h"
#include "aim/FutureAim.h"
#include "colors/ColorPeletteRepo.h"
#include "database/DatabaseCache.h"
#include "objects/PhysicalUtils.h"
#include "objects/unit/ChargeData.h"
#include "objects/unit/ColorMode.h"
#include "objects/unit/MissleData.h"
#include "player/PlayersManager.h"
#include "scene/load/dbload_container.h"
#include "simulation/formation/FormationManager.h"
#include "state/StateManager.h"
#include <Urho3D/Graphics/Material.h><Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h><Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h><Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Resource/ResourceCache.h><Urho3D/Resource/ResourceCache.h>
#include <string>


Unit::Unit(Vector3* _position, int id, int player, int level) : Physical(_position, ObjectType::UNIT),
	state(UnitState::STOP) {
	initBillbords();

	dbUnit = Game::getDatabaseCache()->getUnit(id);
	dbLevel = Game::getDatabaseCache()->getUnitLevel(id, level).value();
	populate();
	if (StateManager::validateState(getDbID(), UnitState::CHARGE)) {
		chargeData = new ChargeData(150, 2);
	}

	if (StateManager::validateState(getDbID(), UnitState::SHOT)) {
		missleData = new MissleData(150, 2);
	}

	node->Scale(dbLevel->scale);
	model = node->CreateComponent<StaticModel>();
	model->SetModel(Game::getCache()->GetResource<Model>("Models/" + dbLevel->model));
	basic = Game::getCache()->GetResource<Material>("Materials/" + dbLevel->texture);
	model->SetMaterial(basic);

	setPlayer(player);
	setTeam(Game::getPlayersManager()->getPlayer(player)->getTeam());

	updateBillbords();
}

Unit::~Unit() {
	delete chargeData;
}

bool Unit::isAlive() const {
	return state != UnitState::DEAD && state != UnitState::DISPOSE;
}

void Unit::populate() {
	maxSeparationDistance = dbLevel->maxSep;
	mass = dbLevel->mass;
	maxSpeed = dbLevel->maxSpeed;
	minSpeed = dbLevel->minSpeed;
	minimalDistance = dbLevel->minDist;
	attackRange = dbLevel->attackRange;
	attackIntrest = dbLevel->attackRange * 10;
	rotatable = dbUnit->rotatable;
	actionState = UnitState(dbUnit->actionState);

	attackIntrest = 10;
	collectSpeed = dbLevel->collectSpeed;

	hpCoef = dbLevel->maxHp;
	maxHpCoef = dbLevel->maxHp;
	attackCoef = dbLevel->attack;
	attackRange = dbLevel->attackRange;
	defenseCoef = dbLevel->defense;
	attackSpeed = dbLevel->attackSpeed / 100.f;
}

void Unit::checkAim() {
	if (aims.ifReach(this)) {
		StateManager::changeState(this, UnitState::MOVE);
	}
}

void Unit::move(double timeStep) {
	if (state != UnitState::STOP) {
		position->x_ += velocity.x_ * timeStep;
		position->z_ += velocity.y_ * timeStep;
		node->SetPosition(*position);
		//node->Translate((*velocity) * timeStep, TS_WORLD);
	}
	if (missleData && missleData->isUp()) {
		missleData->update(timeStep, attackCoef);
	}
}

void Unit::setAcceleration(Vector2& _acceleration) {
	acceleration = _acceleration;
	if (acceleration.LengthSquared() > dbLevel->maxForce * dbLevel->maxForce) {
		acceleration.Normalize();
		acceleration *= dbLevel->maxForce;
	}
}

Vector2 Unit::forceGo(float boostCoef, float aimCoef, Vector2& force) {
	force.Normalize();
	force *= boostCoef;
	force -= velocity;
	force /= 0.5;
	force *= mass;
	force *= aimCoef;
	return force;
}

Vector2 Unit::getDestination(float boostCoef, float aimCoef) {
	Vector2 force;
	aims.clearExpired();
	if (aims.hasAim()) {
		auto dirOpt = aims.getDirection(this);
		if (dirOpt.has_value()) {
			force = dirOpt.value();
			forceGo(boostCoef, aimCoef, force);
		}
	}

	return force;
}

void Unit::absorbAttack(float attackCoef) {
	hpCoef -= attackCoef * (1 - defenseCoef);
	
	updateHealthBar();
	
	if (hpCoef < 0) {
		StateManager::changeState(this, UnitState::DEAD);
	}
}

void Unit::attackIfCloseEnough(float distance, Physical* closest) {
	if (closest) {
		if (distance < attackRange * attackRange) {
			toAttack(closest);
		} else if (distance < attackIntrest * attackIntrest) {
			addAim(FutureAim(closest, UnitOrder::FOLLOW));
		}
	}
}

void Unit::collectIfCloseEnough(float distance, Physical* closest) {
	if (closest) {
		if (distance < attackRange * attackRange) {
			toCollect(closest);
		} else if (distance < attackIntrest * attackIntrest) {
			addAim(FutureAim(closest, UnitOrder::FOLLOW));
		}
	}
}

void Unit::shotIfCloseEnough(float distance, Physical* closest) {
	if (closest) {
		if (distance < attackRange * attackRange) {
			toShot(closest);
		}
	}
}

std::tuple<Physical*, float> Unit::closestEntity(std::vector<Physical*>* enemies, std::function<bool(Physical*)> func) {
	float minDistance = 99999;
	Physical* entityClosest = nullptr;
	for (auto entity : *enemies) {
		if (entity->isAlive() && func(entity)) {
			const float distance = sqDist(this, entity);
			if (distance <= minDistance) {
				minDistance = distance;
				entityClosest = entity;
			}
		}
	}
	return {entityClosest, minDistance};
}

void Unit::toAttack(std::vector<Physical*>* enemies) {
	auto [closest,minDistance] = closestEntity(enemies, belowClose);

	attackIfCloseEnough(minDistance, closest);
}

void Unit::toAttack(Physical* enemy) {
	oneToInteract(enemy);
	toAttack();
}

void Unit::toAttack() {
	StateManager::changeState(this, UnitState::ATTACK);
}

void Unit::toShot(std::vector<Physical*>* enemies) {
	auto [closest,minDistance] = closestEntity(enemies, belowRange);

	shotIfCloseEnough(minDistance, closest);
}

void Unit::oneToInteract(Physical* enemy) {
	thingsToInteract.clear();
	thingsToInteract.push_back(enemy);
}

void Unit::toShot(Physical* enemy) {
	oneToInteract(enemy);
	toShot();
}

void Unit::toShot() {
	StateManager::changeState(this, UnitState::SHOT);
}

void Unit::toCollect(std::vector<Physical*>* entities) {
	float minDistance = 99999;
	Physical* closest = nullptr;
	for (auto physical : *entities) {
		if (physical->belowCloseLimit()) {
			auto pos = physical->getPosToFollow(position);
			const float distance = sqDist(*position, pos);
			if (distance <= minDistance) {
				minDistance = distance;
				closest = physical;
			}
		}
	}
	collectIfCloseEnough(minDistance, closest);
}

void Unit::toCollect() {
	StateManager::changeState(this, UnitState::COLLECT);
}

void Unit::toCharge(std::vector<Physical*>* enemies) {
	thingsToInteract.clear();
	for (auto entity : *enemies) {
		if (entity->isAlive()) {
			const float distance = sqDist(this, entity);
			if (distance <= chargeData->attackRange * chargeData->attackRange) {
				thingsToInteract.push_back(entity);
			}
		}
	}
}

void Unit::toCollect(Physical* _resource) {
	oneToInteract(_resource);
	StateManager::changeState(this, UnitState::COLLECT);
}

void Unit::updateHeight(float y, double timeStep) {
	velocity *= 1 + (position->y_ - y) * 0.1 * mass * timeStep;
	position->y_ = y;
}

void Unit::addAim(Aim* aim) {
	aims.add(aim);
}

void Unit::addAim(const FutureAim& aim, bool append) {
	if (!append) {
		clearAims();
	}
	aims.add(aim);
}

void Unit::clearAims() {
	aims.clear();
}

void Unit::removeCurrentAim() {
	aims.removeCurrentAim();
}

String& Unit::toMultiLineString() {
	menuString = dbUnit->name + " " + dbLevel->name;
	menuString.Append("\nAtak: ").Append(String(attackCoef));
	menuString.Append("\nObrona: ").Append(String(defenseCoef));
	menuString.Append("\nZdrowie: ").Append(String(hpCoef)).Append("/").Append(String(maxHpCoef));
	return menuString;
}

void Unit::action(char id, ActionParameter& parameter) {
	const UnitOrder type = UnitOrder(id);

	switch (type) {
	case UnitOrder::GO:
		StateManager::changeState(this, UnitState::GO_TO, parameter);
		break;
	case UnitOrder::STOP:
		StateManager::changeState(this, UnitState::STOP);
		break;
	case UnitOrder::CHARGE:
		StateManager::changeState(this, UnitState::CHARGE, parameter);
		break;
	case UnitOrder::ATTACK: break;
	case UnitOrder::DEAD:
		StateManager::changeState(this, UnitState::DEAD);
		break;
	case UnitOrder::DEFEND:
		StateManager::changeState(this, UnitState::DEFEND);
		break;
	case UnitOrder::FOLLOW:
		StateManager::changeState(this, UnitState::FOLLOW, parameter);
		break;
	default: ;
	}
}

std::string Unit::getValues(int precision) {
	const int position_x = position->x_ * precision;
	const int position_z = position->z_ * precision;
	const int state = static_cast<int>(this->state);
	const int velocity_x = velocity.x_ * precision;
	const int velocity_z = velocity.y_ * precision;
	return Physical::getValues(precision)
		+ to_string(position_x) + "," +
		to_string(position_z) + "," +
		to_string(state) + "," +
		to_string(velocity_x) + "," +
		to_string(velocity_z) + "," +
		to_string(-1);

}

void Unit::addUpgrade(db_unit_upgrade* upgrade) {
	for (auto& i : upgrades) {
		if (i->path == upgrade->path) {
			i = upgrade;
			return;
		}
	}
	upgrades.push_back(upgrade);
}

void Unit::changeColor(float value, float maxValue) const {
	Material* newMaterial = Game::getColorPeletteRepo()->getColor(ColorPallet::RED, value, maxValue);
	Material* current = model->GetMaterial(0);

	if (newMaterial != current) {
		model->SetMaterial(newMaterial);
	}
}

void Unit::changeColor(Material* newMaterial) {
	Material* current = model->GetMaterial(0);

	if (newMaterial != current) {
		model->SetMaterial(newMaterial);
	}
}

void Unit::changeColor(UnitState state) {
	changeColor(Game::getColorPeletteRepo()->getColor(state));
}

void Unit::changeColor(ColorMode mode) {
	switch (mode) {

	case ColorMode::BASIC:
		{
		Material* current = model->GetMaterial(0);
		if (current != basic) {
			model->SetMaterial(basic);
		}
		}
		break;
	case ColorMode::VELOCITY:
		{
		changeColor(velocity.LengthSquared(), maxSpeed * maxSpeed);
		}
		break;
	case ColorMode::STATE:
		{
		changeColor(state);
		}
		break;
	case ColorMode::FORMATION:
		{
		if (formation != -1) {
			auto prioryty = Game::getFormationManager()->getPriority(this);

			changeColor(prioryty, 3.0f);
		}
		}
		break;
	default: ;
	}
}

void Unit::clean() {
	Physical::clean();
}

void Unit::setState(UnitState _state) {
	state = _state;
}

bool Unit::checkTransition(UnitState state) {
	return StateManager::checkChangeState(this, state);
}

bool Unit::hasResource() {
	return isFirstThingAlive();
}

void Unit::load(dbload_unit* unit) {
	state = UnitState(unit->state); //TODO nie wiem czy nie przepisaŠpoprzez przejscie?
	//aimIndex =unit->aim_i;
	velocity = Vector2(unit->vel_x, unit->vel_z);
	//alive = unit->alive;
	hpCoef = maxHpCoef * unit->hp_coef;
}

int Unit::getLevel() {
	return dbLevel->level;
}

float Unit::getMaxHpBarSize() {
	return 0.4f;
}

void Unit::setFormation(short _formation) {
	formation = _formation;
}

void Unit::resetFormation() {
	formation = -1;
	posInFormation = -1;
}

void Unit::setPositionInFormation(short _pos) {
	posInFormation = _pos;
}

std::string Unit::getColumns() {
	return Physical::getColumns() +
		"position_x		INT     NOT NULL,"
		"position_z		INT     NOT NULL,"
		"state			INT     NOT NULL,"
		"velocity_x		INT     NOT NULL,"
		"velocity_z		INT     NOT NULL,"
		"aim_i		INT     NOT NULL";
}

void Unit::applyForce(double timeStep) {
	if (state == UnitState::ATTACK) {
		return;
	}

	velocity *= 0.5f; //TODO to dac jaki wspolczynnik tarcia terenu
	velocity += acceleration * (timeStep / mass);
	float velLenght = velocity.LengthSquared();
	if (velLenght < minSpeed * minSpeed) {
		if (state == UnitState::MOVE) {
			StateManager::changeState(this, UnitState::STOP);
		}
	} else {
		if (velLenght > maxSpeed * maxSpeed) {
			velocity.Normalize();
			velocity *= maxSpeed;
		}
		if (state == UnitState::STOP) {
			StateManager::changeState(this, UnitState::MOVE);
		}
		if (rotatable && velLenght > 2 * minSpeed * minSpeed) {
			node->SetDirection(Vector3(velocity.x_, 0, velocity.y_));
		}
	}
}

int Unit::getDbID() {
	return dbUnit->id;
}
