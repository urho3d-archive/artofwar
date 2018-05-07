#include "Controls.h"
#include "ControlsUtils.h"
#include "Game.h"
#include "HitData.h"
#include "camera/CameraManager.h"
#include "commands/CommandList.h"
#include "commands/action/ActionCommand.h"
#include "commands/action/GroupAction.h"
#include "commands/creation/CreationCommandList.h"
#include "database/DatabaseCache.h"
#include "player/PlayersManager.h"
#include "simulation/env/Enviroment.h"
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/UI/UI.h>
#include <algorithm>
#include <queue>


Controls::Controls(Input* _input) {
	selected = new std::vector<Physical*>();
	selected->reserve(5000);

	input = _input;
	selectedInfo = new SelectedInfo();

	createNode("Models/box.mdl", "Materials/green_alpha.xml", &selectionNode);
	createNode("Models/arrow2.mdl", "Materials/dark_red_alpha.xml", &arrowNode);

	tempBuildingNode = Game::get()->getScene()->CreateChild();
	tempBuildingModel = tempBuildingNode->CreateComponent<StaticModel>();
	tempBuildingNode->SetEnabled(false);

	selectedType = ObjectType::PHISICAL;
}


Controls::~Controls() {
	delete selectedInfo;
	delete selected;
	selectionNode->Remove();
	arrowNode->Remove();
}

void Controls::unSelectAll() {
	for (auto& phy : *selected) {
		phy->unSelect();
	}
	selected->clear();
	selectedType = ObjectType::ENTITY;
	selectedInfo->setSelectedType(selectedType);
	selectedInfo->reset();
}

void Controls::select(Physical* entity) {
	const ObjectType entityType = entity->getType();
	if (entityType != selectedType) {
		unSelectAll();
	}

	entity->select();
	selected->push_back(entity);

	selectedType = entityType;
	selectedInfo->setSelectedType(selectedType);
	selectedInfo->setAllNumber(selected->size());
	selectedInfo->select(entity);
}

void Controls::leftClick(Physical* clicked, Vector3& hitPos) {
	if (!input->GetKeyDown(KEY_CTRL)) {
		unSelectAll();
	}
	select(clicked);
}

void Controls::leftClickBuild(Physical* clicked, Vector3& hitPos) {
	unSelectAll();
	createBuilding(Vector2(hitPos.x_, hitPos.z_));
}

void Controls::rightClickDefault(Physical* clicked, Vector3& hitPos, bool shiftPressed) {
	switch (clicked->getType()) {
	case ObjectType::PHISICAL:
		{
		OrderType type;
		if (shiftPressed) {
			type = OrderType::PATROL;
		} else {
			type = OrderType::GO;
		}

		Game::get()->getActionCommandList()->add(new GroupAction(selected, type, new Vector2(hitPos.x_, hitPos.z_),
		                                                         shiftPressed));
		break;
		}
	case ObjectType::UNIT:
		{
		Game::get()->getActionCommandList()->add(new GroupAction(selected, OrderType::FOLLOW, clicked, shiftPressed));
		//unSelectAll();
		break;
		}
	case ObjectType::BUILDING:
		Game::get()->getActionCommandList()->add(new GroupAction(selected, OrderType::FOLLOW, clicked, shiftPressed));
		break;
	case ObjectType::RESOURCE: break;
	default: ;
	}
}

void Controls::leftHold(std::pair<Vector3*, Vector3*>& held) {
	if (!input->GetKeyDown(KEY_CTRL)) {
		unSelectAll();
	}
	std::vector<Physical*>* entities = Game::get()->getEnviroment()->getNeighbours(held);
	for (auto entity : *entities) {
		select(entity); //TODO zastapic wrzuceniem na raz
	}
}

void Controls::rightHold(std::pair<Vector3*, Vector3*>& held) {
	auto actions = Game::get()->getActionCommandList();

	auto first = new Vector2(held.first->x_, held.first->z_);
	if (input->GetKeyDown(KEY_SHIFT)) {
		const auto second = new Vector2(held.second->x_, held.second->z_);
		actions->add(new GroupAction(selected, OrderType::PATROL, first));
		actions->add(new GroupAction(selected, OrderType::PATROL, second, true));
	} else {
		const auto charge = new Vector2(held.second->x_ - held.first->x_, held.second->z_ - held.first->z_);

		actions->add(new GroupAction(selected, OrderType::GO, first));
		actions->add(new GroupAction(selected, OrderType::CHARGE, charge, true));
	}
}


void Controls::releaseLeft() {
	hit_data hitData;

	if (raycast(hitData)) {
		left.setSecond(hitData.position);
		const float dist = (*left.held.first - *left.held.second).LengthSquared();
		if (dist > clickDistance) {
			leftHold(left.held);
		} else {
			LinkComponent* lc = hitData.link;
			if (lc) {
				leftClick(lc->getPhysical(), hitData.position);
			}
		}
		selectionNode->SetEnabled(false);
		left.clean();
	}
}

void Controls::releaseBuildLeft() {
	hit_data hitData;

	if (raycast(hitData)) {
		LinkComponent* lc = hitData.link;
		if (lc) {
			leftClickBuild(lc->getPhysical(), hitData.position);
		}
	}
}

void Controls::releaseRight() {
	hit_data hitData;

	if (raycast(hitData)) {
		right.setSecond(hitData.position);
		float dist = (*right.held.first - *right.held.second).LengthSquared();
		if (dist > clickDistance) {
			rightHold(right.held);
		} else {
			LinkComponent* lc = hitData.link;
			if (lc) {
				rightClickDefault(lc->getPhysical(), hitData.position, input->GetKeyDown(KEY_SHIFT));
			}
		}
		arrowNode->SetEnabled(false);
		right.clean();
	}
}

bool Controls::orderAction(bool shiftPressed) {
	hit_data hitData;

	if (raycast(hitData)) {
		LinkComponent* lc = hitData.link;
		if (lc) {
			rightClickDefault(lc->getPhysical(), hitData.position, shiftPressed);
			return true;
		}
	}
	return false;
}

void Controls::resetState() {
	if (selectedInfo->hasChanged()) {
		//TODO to dziwny warunek
		toDefault();
	}
}

void Controls::hudAction(HudData* hud) {
	state = BUILD;
	typeToCreate = ObjectType::BUILDING;
	idToCreate = hud->getId();
}

void Controls::order(short id, ActionParameter& parameter) {
	switch (selectedType) {
	case ObjectType::PHISICAL:
		orderPhysical(id, parameter);
		break;
	case ObjectType::UNIT:
		actionUnit(id, parameter);
		break;
	case ObjectType::BUILDING:
		orderBuilding(id, parameter);
		break;
	case ObjectType::RESOURCE: break;
	default: ;
	}
}

void Controls::orderBuilding(short id, ActionParameter& parameter) {
	for (auto& phy : *selected) {
		phy->action(id, parameter);
	}
}

void Controls::orderPhysical(short id, ActionParameter& parameter) {
	const int level = Game::get()->getPlayersManager()->getActivePlayer()->getLevelForBuilding(id) + 1;

	Resources& resources = Game::get()->getPlayersManager()->getActivePlayer()->getResources();

	switch (parameter.type) {
	case ActionType::BUILDING_LEVEL:
		{
		optional<std::vector<db_cost*>*> opt = Game::get()->getDatabaseCache()->getCostForBuildingLevel(id, level);
		if (opt.has_value()) {
			auto costs = opt.value();
			if (resources.reduce(costs)) {
				Game::get()->getQueueManager()->add(1, parameter.type, id, 1);
			}
		}
		break;
		}
	}
}

void Controls::startSelectionNode(hit_data hitData) {
	selectionNode->SetEnabled(true);
	selectionNode->SetScale(1);
	int y = selectionNode->GetPosition().y_;
	Vector3 newPos = hitData.position;
	newPos.y_ = y;
	selectionNode->SetPosition(newPos);
}

void Controls::startArrowNode(const hit_data& hitData) {
	arrowNode->SetEnabled(true);
	arrowNode->SetScale(1);
	int y = arrowNode->GetPosition().y_;
	Vector3 newPos = hitData.position;
	newPos.y_ = y;
	arrowNode->SetPosition(newPos);
}

bool Controls::clickDown(MouseButton& var, hit_data hitData) {
	bool clicked = raycast(hitData);
	if (clicked) {
		var.setFirst(hitData.position);
	}
	return clicked;
}

void Controls::clickDownLeft() {
	hit_data hitData;
	const bool clicked = clickDown(left, hitData);
	if (clicked) {
		startSelectionNode(hitData);
	}
}

void Controls::clickDownRight() {
	hit_data hitData;
	bool clicked = clickDown(right, hitData);
	if (clicked) {
		startArrowNode(hitData);
	}
}

void Controls::createBuilding(Vector2& pos) {
	if (idToCreate >= 0) {
		auto player = Game::get()->getPlayersManager()->getActivePlayer();

		Game::get()->getCreationCommandList()->addBuilding(
		                                                   idToCreate, pos,
		                                                   player->getId(),
		                                                   player->getLevelForBuilding(idToCreate)
		                                                  );
	}
}

SelectedInfo* Controls::getInfo() {
	return selectedInfo;
}

ControlsState Controls::getState() {
	return state;
}

void Controls::cleanMouse() {
	left.clean();
	right.clean();
	selectionNode->SetEnabled(false);
	arrowNode->SetEnabled(false);
}

void Controls::deactivate() {
	if (active) {
		active = true;
		cleanMouse();
	}
}

void Controls::activate() {
	if (!active) {
		active = true;
		cleanMouse();
	}
}

void Controls::unitOrder(short id) {
	auto type = OrderType(id);
	switch (type) {
	case OrderType::GO:
	case OrderType::CHARGE:
	case OrderType::ATTACK:
	case OrderType::PATROL:
	case OrderType::FOLLOW:
		state = ORDER;
		orderType = type;
		break;
	case OrderType::STOP:
	case OrderType::DEFEND:
	case OrderType::DEAD:
		for (auto& phy : *selected) {
			ActionParameter empty;
			phy->action(id, empty); //TODO przemyslec to
		}
		break;
	default: ;
	}
}

void Controls::actionUnit(short id, ActionParameter& parameter) {
	switch (parameter.type) {
	case ActionType::ORDER:
		unitOrder(id);
		break;
	case ActionType::FORMATION:
		unitFormation(id);
		break;
	default: ;
	}
}

void Controls::refreshSelected() {
	selected->erase(
	                std::remove_if(
	                               selected->begin(), selected->end(),
	                               [](Physical* physical)
	                               {
		                               if (!physical->isAlive()) {
			                               physical->unSelect();
			                               return true;
		                               }
		                               return false;
	                               }),
	                selected->end());
}

void Controls::clean(SimulationInfo* simulationInfo) {
	bool condition;
	switch (selectedType) {

	case ObjectType::UNIT:
		condition = simulationInfo->ifUnitDied();
		break;
	case ObjectType::BUILDING:
		condition = simulationInfo->ifBuildingDied();
		break;
	case ObjectType::RESOURCE:
		condition = simulationInfo->ifResourceDied();
		break;
	default:
		condition = false;
	}

	if (condition) {
		refreshSelected();
	}
	resetState();
}

void Controls::updateSelection() {
	hit_data hitData;

	if (raycast(hitData)) {
		float xScale = left.held.first->x_ - hitData.position.x_;
		float zScale = left.held.first->z_ - hitData.position.z_;

		selectionNode->SetScale(Vector3(abs(xScale), 0.5, abs(zScale)));
		Vector3 center = (*left.held.first + hitData.position) / 2;
		center.y_ += 1;
		selectionNode->SetPosition(center);
	}
}

void Controls::updateArrow() {
	hit_data hitData;

	if (raycast(hitData)) {
		Vector3 dir = *right.held.first - hitData.position;

		Vector3 pos = *right.held.first;
		float length = dir.Length();
		arrowNode->SetScale(Vector3(length, 1, length / 3));
		arrowNode->SetDirection(Vector3(-dir.z_, 0, dir.x_));
		pos.y_ += 1;
		arrowNode->SetPosition(pos);
	}
}

void Controls::toDefault() {
	state = DEFAULT;
	cleanMouse();
	idToCreate = -1;
	tempBuildingNode->SetEnabled(false);
}

void Controls::defaultControl() {
	if (input->GetMouseButtonDown(MOUSEB_LEFT)) {
		if (!left.isHeld) {
			clickDownLeft();
		} else {
			updateSelection();
		}
	} else if (left.isHeld) {
		releaseLeft();
	}

	if (input->GetMouseButtonDown(MOUSEB_RIGHT)) {
		if (!right.isHeld) {
			clickDownRight();
		} else {
			updateArrow();
		}
	} else if (right.isHeld) {
		releaseRight();
	}
	if (input->GetMouseButtonDown(MOUSEB_MIDDLE)) {
		hit_data hitData;

		if (raycast(hitData)) {
			selectedInfo->setMessage(hitData.position.ToString());
		}
	}
}

void Controls::buildControl() {
	if (!input->GetMouseButtonDown(MOUSEB_LEFT) && !input->GetMouseButtonDown(MOUSEB_RIGHT)) {
		hit_data hitData;

		if (raycast(hitData)) {
			auto env = Game::get()->getEnviroment();

			auto dbBuilding = Game::get()->getDatabaseCache()->getBuilding(idToCreate);
			auto dbLevel = Game::get()->getDatabaseCache()->getBuildingLevel(dbBuilding->id, 0).value();
			Vector2 post3 = Vector2(hitData.position.x_, hitData.position.z_);
			bool ifPosible = env->validateStatic(dbBuilding->size, post3);

			Vector2 pos = env->getValidPosition(dbBuilding->size, post3);
			float y = env->getGroundHeightAt(pos.x_, pos.y_);

			Vector3 pos2 = Vector3(pos.x_, y, pos.y_);
			tempBuildingNode->SetPosition(pos2);
			if (!tempBuildingNode->IsEnabled()) {
				tempBuildingNode->SetScale(dbLevel->scale);

				tempBuildingModel->SetModel(Game::get()->getCache()->GetResource<Urho3D::Model>("Models/" + dbLevel->model));
				tempBuildingNode->SetEnabled(true);
			}
			if (ifPosible) {
				tempBuildingModel->
					SetMaterial(Game::get()->getCache()->GetResource<Urho3D::Material>("Materials/green_alpha.xml"));
			} else {
				tempBuildingModel->
					SetMaterial(Game::get()->getCache()->GetResource<Urho3D::Material>("Materials/dark_red_alpha.xml"));
			}

		}
	}

	if (input->GetMouseButtonDown(MOUSEB_LEFT)) {
		if (!left.isHeld) {
			left.markHeld();
		}
	} else if (left.isHeld) {
		releaseBuildLeft();
	}

	if (input->GetMouseButtonDown(MOUSEB_RIGHT)) {
		toDefault();
	}

}

void Controls::orderControl() {
	if (input->GetMouseButtonDown(MOUSEB_LEFT)) {
		if (!left.isHeld) {
			left.markHeld();
		}
	} else if (left.isHeld) {
		switch (orderType) {
		case OrderType::GO:
		case OrderType::ATTACK:
		case OrderType::FOLLOW:
			if (orderAction(false)) {
				toDefault();
			}
			break;
		case OrderType::PATROL:
			orderAction(true);
			break;
		case OrderType::CHARGE:

			break;
		default: ;
		}
		left.clean();
	}

	if (input->GetMouseButtonDown(MOUSEB_RIGHT)) {
		if (!right.isHeld) {
			right.markHeld();
		}
	} else if (right.isHeld) {
		toDefault();
	}
}

void Controls::control() {
	switch (state) {
	case DEFAULT:
		defaultControl();
		break;
	case BUILD:
		buildControl();
		break;
	case ORDER:
		orderControl();
		break;
	}
}

void Controls::unitFormation(short id) {
	Game::get()->getFormationManager()->createFormation(selected, FormationType(id));
}
