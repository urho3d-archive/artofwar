#include "Simulation.h"
#include "Game.h"
#include "objects/unit/ColorMode.h"
#include "commands/creation/CreationCommand.h"
#include "commands/creation/CreationCommandList.h"
#include "commands/upgrade/UpgradeCommand.h"
#include "player/PlayersManager.h"
#include "scene/load/SceneLoader.h"
#include "simulation/formation/FormationManager.h"
#include "OperatorType.h"
#include "env/Enviroment.h"
#include "scene/save/SceneSaver.h"
#include "objects/queue/QueueElement.h"
#include "objects/unit/state/UnitState.h"
#include "objects/unit/Unit.h"
#include "objects/MenuAction.h"
#include <ctime>
#include "objects/unit/state/StateManager.h"


Simulation::Simulation(Enviroment* _enviroment, CreationCommandList* _creationCommandList) {
	enviroment = _enviroment;
	simObjectManager = _creationCommandList->getManager();
	creationCommandList = _creationCommandList;
	levelsCommandList = new UpgradeCommandList(simObjectManager);
	currentColor = ColorMode::BASIC;


	srand(time(NULL));

	simulationInfo = new SimulationInfo();
	actionCommandList = new CommandList();
	Game::setActionCommmandList(actionCommandList);

	units = simObjectManager->getUnits();
	buildings = simObjectManager->getBuildings();
	resources = simObjectManager->getResources();
}

Simulation::~Simulation() {
	delete simulationInfo;
	delete actionCommandList;
	delete levelsCommandList;
	Game::setActionCommmandList(nullptr);
}

void Simulation::tryToAttack(Unit* unit) {
	if (unit->hasEnemy()) {
		unit->toAttack();
	} else {
		unit->toAttack(enviroment->getNeighboursFromTeam(unit, 12, unit->getTeam(), OperatorType::NOT_EQUAL));
	}
}

void Simulation::tryToCollect(Unit* unit) {
	if (unit->hasResource()) {
		unit->toCollect();
	} else {
		unit->toCollect(enviroment->getResources(unit, 12));
	}
}

void Simulation::tryToShot(Unit* unit) {
	if (unit->hasEnemy()) {
		unit->toShot();
	} else {
		unit->toShot(enviroment->getNeighboursFromTeam(unit, 12, unit->getTeam(), OperatorType::NOT_EQUAL));
	}
}

void Simulation::selfAI() {
	for (auto unit : *units) {
		switch (unit->getState()) {
		case UnitState::CHARGE:
			unit->toCharge(enviroment->getNeighboursFromTeam(unit, 12, unit->getTeam(), OperatorType::NOT_EQUAL));
			break;
		case UnitState::STOP:
		case UnitState::MOVE:
			if (currentFrameNumber % 3 == 0 && unit->getFormation() == -1
				&& unit->checkTransition(unit->getActionState())) {
				switch (unit->getActionState()) {
				case UnitState::ATTACK:
					tryToAttack(unit);
					break;
				case UnitState::COLLECT:
					tryToCollect(unit);
					break;
				case UnitState::SHOT:
					tryToShot(unit);
					break;
				}
			}
			break;
		}

	}
}

void Simulation::loadEntities(SceneLoader& loader) const {
	dbload_container* data = loader.getData();
	for (auto unit : *data->units) {
		simObjectManager->load(unit);
	}
	for (auto resource : *data->resource_entities) {
		simObjectManager->load(resource);
	}
	for (auto building : *data->buildings) {
		simObjectManager->load(building);
	}
}

void Simulation::addTestEntities() const {
	if constexpr (UNITS_NUMBER > 0) {
		creationCommandList->addUnits(UNITS_NUMBER, 4, Vector2(20, 0), 0, 0);
		creationCommandList->addResource(0, Vector2(0, 0), 0);
	}
}

void Simulation::loadEntities(NewGameForm* form) const {
	for (const auto& player : form->players) {
		simObjectManager->addUnits(10, 1, Vector2(), player.id, 0);
	}
}

float Simulation::updateTime(float timeStep) {
	if (timeStep > maxTimeFrame) {
		timeStep = maxTimeFrame;
	}
	accumulateTime += timeStep;
	return timeStep;
}

void Simulation::countFrame() {
	++currentFrameNumber;
	if (currentFrameNumber >= framesPeriod) {
		currentFrameNumber = 0;
	}
}

void Simulation::applyForce() {
	for (auto unit : *units) {
		unit->applyForce(maxTimeFrame);
		Vector3* pos = unit->getPosition();
		//TODO to przeniesc do mova? to moze byc [rpblem gdy jest przesuwanie poza klatk�
		const float y = enviroment->getGroundHeightAt(pos->x_, pos->z_);
		unit->updateHeight(y, maxTimeFrame);
	}
}

void Simulation::levelUp(QueueElement* done) const {
	levelsCommandList->add(new UpgradeCommand(
	                                          Game::getPlayersManager()->getActivePlayer()->getId(),
	                                          done->getId(),
	                                          done->getType()
	                                         ));
}

void Simulation::updateBuildingQueues(const float time) {
	for (auto build : *buildings) {
		QueueElement* done = build->updateQueue(time);
		if (done) {
			switch (done->getType()) {
			case MenuAction::UNIT:
				creationCommandList->add(new CreationCommand(
				                                             ObjectType::UNIT,
				                                             done->getAmount(),
				                                             done->getId(),
				                                             build->getTarget(),
				                                             build->getPlayer(),
				                                             Game::getPlayersManager()->
				                                                          getPlayer(build->getPlayer())->
				                                                          getLevelForUnit(done->getId())
				                                            ));
				break;
			case MenuAction::UNIT_LEVEL:
			case MenuAction::BUILDING_LEVEL:
			case MenuAction::UNIT_UPGRADE:
				levelUp(done);
				break;
			}

			delete done;
		}
	}
}

void Simulation::updateQueues() {
	updateBuildingQueues(maxTimeFrame);
	QueueElement* done = Game::getQueueManager()->update(maxTimeFrame);
	if (done) {
		switch (done->getType()) {
		case MenuAction::BUILDING_LEVEL:
			levelUp(done);
			break;
		}
		delete done;
	}
}

void Simulation::dispose() {
	simObjectManager->dispose();
}

void Simulation::save(SceneSaver& saver) {
	saver.saveUnits(units);
	saver.saveBuildings(buildings);
	saver.saveResourceEntities(resources);
}

void Simulation::changeCoef(int i, int wheel) {
	force.changeCoef(i, wheel);
}

void Simulation::changeColorMode(ColorMode _colorMode) {
	currentColor = _colorMode;
}

void Simulation::performStateAction() {
	for (auto unit : *units) {
		StateManager::execute(unit);
	}
}

void Simulation::handleTimeInFrame(float timeStep) {
	countFrame();
	moveUnits(maxTimeFrame - (accumulateTime - timeStep));
	accumulateTime -= maxTimeFrame;
}

SimulationInfo* Simulation::update(float timeStep) {
	simulationInfo->reset();
	timeStep = updateTime(timeStep);
	if (accumulateTime >= maxTimeFrame) {
		handleTimeInFrame(timeStep);

		executeLists();
		selfAI();

		actionCommandList->execute();
		enviroment->update(units);

		calculateForces();
		applyForce();

		moveUnitsAndCheck(accumulateTime);

		performStateAction();
		updateQueues();

		simObjectManager->prepareToDispose();
		simObjectManager->updateInfo(simulationInfo);

		Game::getFormationManager()->update();
	} else {
		moveUnits(timeStep);
	}
	return simulationInfo;
}

void Simulation::executeLists() const {
	levelsCommandList->execute();
	creationCommandList->execute();
}

void Simulation::initScene(SceneLoader& loader) {
	//loadEntities(loader);
	addTestEntities();
	executeLists();
}

void Simulation::initScene(NewGameForm* form) {
	loadEntities(form);
	addTestEntities();
	executeLists();
}

void Simulation::moveUnits(const float timeStep) {
	for (auto unit : *units) {
		unit->move(timeStep);
	}
}

void Simulation::moveUnitsAndCheck(const float timeStep) {
	for (auto unit : *units) {
		unit->move(timeStep);
		unit->checkAim();
		unit->changeColor(currentColor);
	}
}

void Simulation::calculateForces() {
	for (auto unit : *units) {
		Vector2 newForce;

		std::vector<Physical*>* neighbours = enviroment->getNeighbours(unit, unit->getMaxSeparationDistance());

		force.separationUnits(newForce, unit, neighbours);
		force.separationObstacle(newForce, unit, enviroment->repulseObstacle(unit));
		force.destination(newForce, unit);
		force.formation(newForce, unit);
		force.escapeFromInvalidPosition(newForce, enviroment->validatePosition(unit->getPosition()));

		float* stats = force.stats();

		unit->setAcceleration(newForce);
	}
}
