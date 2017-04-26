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
#include "Entity.h"
#include "ObjectEnums.h"
#include <Urho3D/Input/Input.h>

using namespace Urho3D;

class Controls
{
public:
	Controls(Input* _input);
	~Controls();
	bool raycast(Vector3& hitPos, Drawable*& hitDrawable, Camera* camera);
	void unselect(Entity* entity);
	void select(Entity* entity);
	void unSelect(int type);
	void action(ActionType action, Entity* entity);

	void clickDownRight(Vector3 hitPos);
	void clickDownLeft(Vector3 hitPos);
	void clickDown(const int button);

	void clickLeft(Drawable* hitDrawable, Vector3 hitPos, bool ctrlPressed);
	void clickRight(Drawable* hitDrawable, Vector3 hitPos, bool shiftPressed);
	void leftReleased(std::pair<Entity*, Entity*>* held, bool ctrlPressed);
	void rightReleased(std::pair<Entity*, Entity*>* pair, bool shiftPressed);
	void release(const int button);
private:
	std::vector<Entity*>* selected;//TODO to powinien by� set
	ObjectType selectedType;
	float maxDistance = 300;

	bool mouseLeftHeld = false;
	bool mouseRightHeld = false;
	bool mouseMiddleHeld = false;

	std::pair<Entity*, Entity*> *leftHeld;
	std::pair<Entity*, Entity*> *middleHeld;
	std::pair<Entity*, Entity*> *rightHeld;

	Input* input;
	double clickDistance = 2;
};
