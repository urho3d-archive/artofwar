#pragma once

#include "ColorMode.h"
#include "objects/Physical.h"
#include "state/UnitState.h"
#include <Urho3D/Graphics/StaticModel.h>
#include <vector>
#include "aim/Aims.h"


class Aim;
struct db_unit_upgrade;
struct db_unit_level;
struct db_unit;
struct dbload_unit;
struct FutureAim;
class State;
class ResourceEntity;
class MissleData;
class ChargeData;

class Unit : public Physical
{
	friend class State;
	friend class StopState;
	friend class DefendState;
	friend class DeadState;
	friend class GoState;
	friend class PatrolState;
	friend class FollowState;
	friend class AttackState;
	friend class ChargeState;
	friend class CollectState;
	friend class DisposeState;
	friend class ShotState;
public:
	Unit(Vector3* _position, int id, int player, int level);
	~Unit();

	void populate();
	void checkAim();
	void move(double timeStep);
	void applyForce(double timeStep);
	void setAcceleration(Vector2& _acceleration);

	Vector2 forceGo(float boostCoef, float aimCoef, Vector2& force);
	Vector2 getDestination(float boostCoef, float aimCoef);

	void absorbAttack(float attackCoef) override;

	void toAttack(std::vector<Unit*>* enemies);
	void toAttack(Physical* enemy);
	void toAttack();

	void toCollect(std::vector<Physical*>* enemies);
	void toCollect(ResourceEntity* _resource);
	void toCollect();

	void toCharge(std::vector<Unit*>* enemies);

	void updateHeight(float y, double timeStep);
	void addAim(Aim* aim);

	void setState(UnitState _state);
	bool checkTransition(UnitState state);
	void executeState();
	bool hasResource();
	void load(dbload_unit* unit);

	void setFormation(short _formation);
	void resetFormation();
	void setPositionInFormation(short _pos);
	void clearAims();
	void removeCurrentAim();

	static std::string getColumns();
	void addUpgrade(db_unit_upgrade* upgrade);
	void changeColor(ColorMode mode);
	void addAim(const FutureAim& aim, bool append = false);

	float getMaxSeparationDistance() const { return maxSeparationDistance; }
	UnitState getActionState() const { return actionState; }
	short getPositionInFormation() const { return posInFormation; }
	float getMinimalDistance() const { return minimalDistance; }
	UnitState getState() const { return state; }
	short getFormation() const { return formation; }
	bool isToDispose() const { return state == UnitState::DISPOSE && atState; }
	bool hasAim() { return aims.hasAim(); }

	void action(char id, ActionParameter& parameter) override;
	std::string getValues(int precision) override;
	String& toMultiLineString() override;
	float getMaxHpBarSize() override;
	bool isAlive() const override;
	int getLevel() override;
	int getDbID() override;
	void clean() override;
private:

	void attackIfCloseEnough(float distance, Unit* closest);
	void collectIfCloseEnough(float distance, ResourceEntity* closest);

	void changeColor(float value, float maxValue) const;
	void changeColor(Material* newMaterial);
	void changeColor(UnitState state);

	Vector2 acceleration;
	Vector2 velocity;
	Aims aims;

	ResourceEntity* resource;
	Vector2* toResource;
	db_unit* dbUnit;
	db_unit_level* dbLevel;

	UnitState state;
	UnitState actionState;

	UnitState defaultAttackState;

	StaticModel* model;
	Material* basic;

	ChargeData* chargeData{};
	MissleData* missleData{};

	std::vector<db_unit_upgrade*> upgrades;

	bool rotatable;
	bool atState = false;
	float minimalDistance;
	float attackIntrest = 10;
	float collectSpeed = 2;
	float mass;
	float maxSpeed;
	float minSpeed;
	float maxSeparationDistance;

	short posInFormation = -1;
	short formation = -1;

	unsigned short currentFrameState = 0;
};
