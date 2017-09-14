#pragma once

#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Math/Ray.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/OctreeQuery.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <vector>
#include "objects/Entity.h"
#include "ObjectEnums.h"
#include <Urho3D/Input/Input.h>
#include "ControlsState.h"
#include "Game.h"
#include "commands/SimulationCommand.h"
#include "commands/BuildCommand.h"
#include "objects/LinkComponent.h"
#include "Mediator.h"
#include "hud/HudElement.h"
#include "SelectedInfo.h"
#include "OrderType.h"


using namespace Urho3D;

class Controls
{
public:
	Controls(Input* _input);
	~Controls();
	
	void select(Physical* entity);
	void unSelectAll();

	void clickDown(const int button);

	void leftClick(Physical* clicked, Vector3& hitPos);
	void leftHold(std::pair<Vector3*, Vector3*>* held);

	void rightClick(Physical* clicked, Vector3& hitPos);
	void rightHold(std::pair<Vector3*, Vector3*>* pair);

	void release(const int button);
	void updateState(SelectedInfo* selectedInfo);
	void hudAction(HudElement* hud);
	SelectedInfo* getInfo();

	void deactivate();
	bool isActive();
	void activate();
	void action(HudElement* hudElement);
	void clean(SimulationInfo* simulationInfo);

private:
	void create(ObjectType type, Vector3* pos, int number);
	void setFirst(Vector3& hitPos, std::pair<Vector3*, Vector3*>* var);
	void refreshSelected();
	bool raycast(Vector3& hitPos, Drawable*& hitDrawable, Camera* camera);
	void controlEntity(Vector3& hitPos, Physical* clicked);
	void cleanPair(std::pair<Vector3*, Vector3*>* var);

	std::vector<Physical*>* selected;//TODO to powinien by� set

	float maxDistance = 300;

	bool mouseLeftHeld = false;
	bool mouseRightHeld = false;
	bool mouseMiddleHeld = false;

	std::pair<Vector3*, Vector3*>* leftHeld;
	std::pair<Vector3*, Vector3*>* middleHeld;
	std::pair<Vector3*, Vector3*>* rightHeld;

	Input* input;
	ObjectType selectedType;
	ControlsState state = ControlsState::DEFAULT;
	OrderType orderType = OrderType::GO;
	ObjectType typeToCreate;
	SelectedInfo* selectedInfo;
	short idToCreate = -1;
	double clickDistance = 2 * 2;
	bool active = true;

};
