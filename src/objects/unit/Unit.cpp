#include "Unit.h"
#include "Game.h"
#include "OrderType.h"
#include "aim/FutureAim.h"
#include "colors/ColorPeletteRepo.h"
#include "commands/action/ActionCommand.h"
#include "database/DatabaseCache.h"
#include "player/PlayersManager.h"
#include <Urho3D/Graphics/Material.h><Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h><Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h><Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Resource/ResourceCache.h><Urho3D/Resource/ResourceCache.h>
#include <string>
#include "simulation/formation/FormationManager.h"


Unit::Unit(Vector3* _position, int id, int player, int level) : Physical(_position, ObjectType::UNIT), dbUnit(nullptr) {

	initBillbords();

	toResource = new Vector2();
	resource = nullptr;

	state = UnitStateType::STOP;
	dbUnit = Game::get()->getDatabaseCache()->getUnit(id);
	dbLevel = Game::get()->getDatabaseCache()->getUnitLevel(id, level).value();
	populate();

	node->Scale(dbLevel->scale);
	model = node->CreateComponent<StaticModel>();
	model->SetModel(Game::get()->getCache()->GetResource<Model>("Models/" + dbLevel->model));
	basic = Game::get()->getCache()->GetResource<Material>("Materials/" + dbLevel->texture);
	model->SetMaterial(basic);

	setPlayer(player);
	setTeam(Game::get()->getPlayersManager()->getPlayer(player)->getTeam());

	updateBillbords();
}

Unit::~Unit() {
	delete toResource;
}

bool Unit::isAlive() const {
	return state != UnitStateType::DEAD && state != UnitStateType::DISPOSE;
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
	actionState = UnitStateType(dbUnit->actionState);

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
		StateManager::get()->changeState(this, UnitStateType::MOVE);
	}
}

void Unit::move(double timeStep) {
	if (state != UnitStateType::STOP) {
		position->x_ += velocity.x_ * timeStep;
		position->z_ += velocity.y_ * timeStep;
		node->SetPosition(*position);
		//node->Translate((*velocity) * timeStep, TS_WORLD);
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
		} else if (toResource) {
			force = Vector2(
			                toResource->x_,
			                toResource->y_
			               );
			forceGo(boostCoef, aimCoef, force);
		}
	}

	return force;
}

void Unit::absorbAttack(float attackCoef) {
	hpCoef -= attackCoef * (1 - defenseCoef);
	if (billboardBar->enabled_) {
		updateHealthBar();
	}
	if (hpCoef < 0) {
		StateManager::get()->changeState(this, UnitStateType::DEAD);
	}
}

void Unit::attackIfCloseEnough(float& distance, Unit* closest) {
	if (closest) {
		if (distance < attackRange * attackRange) {
			toAttack(closest);
			//attackRange();
		} else if (distance < attackIntrest * attackIntrest) {
			addAim(FutureAim(closest, OrderType::FOLLOW));
			//Game::get()->getActionCommandList()->add(new IndividualAction(this, OrderType::FOLLOW, closest));
		}
	}
}

void Unit::collectIfCloseEnough(float distance, ResourceEntity* closest) {
	if (closest) {
		if (distance < attackRange * attackRange) {
			toCollect(closest);
		} else if (distance < attackIntrest * attackIntrest) {
			addAim(FutureAim(closest, OrderType::FOLLOW));
			//Game::get()->getActionCommandList()->add(new IndividualAction(this, OrderType::FOLLOW, closest));
		}
	}
}

void Unit::toAttack(std::vector<Unit*>* enemies) {
	float minDistance = 9999;
	Unit* entityClosest = nullptr;
	for (auto entity : *enemies) {
		if (entity->isAlive()) {
			float distance = (*this->getPosition() - *entity->getPosition()).LengthSquared();
			if (distance <= minDistance) {
				minDistance = distance;
				entityClosest = entity;
			}
		}
	}
	attackIfCloseEnough(minDistance, entityClosest);
}

void Unit::toAttack(Physical* enemy) {
	thingsToInteract.clear();
	thingsToInteract.push_back(enemy);
}

void Unit::toAttack() {
	StateManager::get()->changeState(this, UnitStateType::ATTACK);
}

void Unit::toCollect(std::vector<Physical*>* enemies) {
	float minDistance = 9999;
	ResourceEntity* entityClosest = nullptr;
	for (auto entity : *enemies) {
		ResourceEntity* resource = dynamic_cast<ResourceEntity*>(entity);
		if (resource->belowLimit()) {
			const float distance = (*this->getPosition() - *entity->getPosition()).LengthSquared();
			if (distance <= minDistance) {
				minDistance = distance;
				entityClosest = resource;
			}
		}
	}
	collectIfCloseEnough(minDistance, entityClosest);
}

void Unit::toCollect() {
	return toCollect(resource);
}

void Unit::toCollect(ResourceEntity* _resource) {
	StateManager::get()->changeState(this, UnitStateType::COLLECT);
	resource = _resource;
}

void Unit::updateHeight(float y, double timeStep) {
	velocity *= 1 + (position->y_ - y) * mass * timeStep;
	position->y_ = y;
}

void Unit::addAim(Aim* aim) {
	aims.add(aim);
}

void Unit::addAim(FutureAim& aim, bool append) {
	if (!append) {
		clearAims();
	}
	aims.add(aim);
}

void Unit::clearAims() {
	aims.clear();
}

String& Unit::toMultiLineString() {
	menuString = dbUnit->name + " " + dbLevel->name;
	menuString.Append("\nAtak: ").Append(String(attackCoef));
	menuString.Append("\nObrona: ").Append(String(defenseCoef));
	menuString.Append("\nZdrowie: ").Append(String(hpCoef)).Append("/").Append(String(maxHpCoef));
	return menuString;
}

void Unit::action(char id, ActionParameter& parameter) {
	const OrderType type = OrderType(id);

	switch (type) {
	case OrderType::GO:
		StateManager::get()->changeState(this, UnitStateType::GO, parameter);
		break;
	case OrderType::STOP:
		StateManager::get()->changeState(this, UnitStateType::STOP);
		break;
	case OrderType::CHARGE:
		StateManager::get()->changeState(this, UnitStateType::CHARAGE, parameter);
		break;
	case OrderType::ATTACK: break;
	case OrderType::PATROL:
		StateManager::get()->changeState(this, UnitStateType::PATROL, parameter);
		break;
	case OrderType::DEAD:
		StateManager::get()->changeState(this, UnitStateType::DEAD);
		break;
	case OrderType::DEFEND:
		StateManager::get()->changeState(this, UnitStateType::DEFEND);
		break;
	case OrderType::FOLLOW:
		StateManager::get()->changeState(this, UnitStateType::FOLLOW, parameter);
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

void Unit::changeColor(float value, float maxValue) {
	Material* newMaterial = Game::get()->getColorPeletteRepo()->getColor(ColorPallet::RED, value, maxValue);
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

void Unit::changeColor(UnitStateType state) {
	Material* newMaterial = Game::get()->getColorPeletteRepo()->getColor(state);
	changeColor(newMaterial);
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
			auto prioryty = Game::get()->getFormationManager()->getPriority(this);

			changeColor(prioryty, 3.0f);
		}
		}
		break;
	default: ;
	}
}

UnitStateType Unit::getActionState() {
	return actionState;
}

void Unit::clean() {
	if (resource != nullptr && !resource->isAlive()) {
		resource = nullptr;
	}
	Physical::clean();
}

void Unit::setState(UnitStateType _state) {
	state = _state;
}

bool Unit::checkTransition(UnitStateType state) {
	return StateManager::get()->checkChangeState(this, state);
}

void Unit::executeState() {
	StateManager::get()->execute(this);
}

bool Unit::hasResource() {
	return resource;
}

void Unit::load(dbload_unit* unit) {
	state = UnitStateType(unit->state); //TODO nie wiem czy nie przepisaŠpoprzez przejscie?
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
	if (state == UnitStateType::ATTACK) {
		return;
	}

	velocity *= 0.5f; //TODO to dac jaki wspolczynnik tarcia terenu
	velocity += acceleration * (timeStep / mass);
	float velLenght = velocity.LengthSquared();
	if (velLenght < minSpeed * minSpeed) {
		StateManager::get()->changeState(this, UnitStateType::STOP);
	} else {
		if (velLenght > maxSpeed * maxSpeed) {
			velocity.Normalize();
			velocity *= maxSpeed;
		}
		StateManager::get()->changeState(this, UnitStateType::MOVE);
		if (rotatable && velLenght > 2 * minSpeed * minSpeed) {
			node->SetDirection(Vector3(velocity.x_, 0, velocity.y_));
		}
	}
}

int Unit::getDbID() {
	return dbUnit->id;
}
