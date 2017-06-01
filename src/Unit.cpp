#include "Unit.h"


Unit::Unit(Vector3* _position, Urho3D::Node* _boxNode, Font* _font) : Entity(_position, _boxNode, _font) {
	acceleration = new Vector3();
	velocity = new Vector3();
	aims = nullptr;
}

Unit::~Unit() {
	//delete node;
	//delete aims;
}

void Unit::populate(db_unit* definition) {
	maxSeparationDistance = definition->maxSep;
	mass = definition->mass;
	maxSpeed = definition->maxSpeed;
	minSpeed = maxSpeed * 0.2f;
	minimalDistance = definition->minDist;
}

void Unit::move(double timeStep) {
	(*position) += (*velocity) * timeStep;
	node->SetPosition(*position);
	(*velocity) *= 0.95;//TODO to dac jaki wspolczynnik tarcia terenu
	if (aims != nullptr) {
		bool reach = aims->ifReach(position, aimIndex);
		if (reach) {
			aimIndex++;
			bool end = aims->check(aimIndex);
			if (end) {
				aims = nullptr;
			}
		}

	}
}

void Unit::setAcceleration(Vector3* _acceleration) {
	delete acceleration;
	acceleration = _acceleration;
}

double Unit::getMaxSeparationDistance() {
	return maxSeparationDistance;
}

void Unit::action(ActionType actionType, ActionParameter* parameter) {
	switch (actionType) {
	case ADD_AIM:
		addAim(parameter);
		break;
	case APPEND_AIM:
		appendAim(parameter);
		break;
	case FOLLOW:
		break;
	default:
		break;//zalogowa�
	}
}

Vector3* Unit::getAim() {
	if (aims == nullptr) {
		return nullptr;
	}
	return aims->getAimPos(aimIndex);
}

Vector3* Unit::getVelocity() {
	return velocity;
}

double Unit::getMass() {
	return mass;
}

double Unit::getUnitRadius() {
	return unitRadius;
}

void Unit::absorbAttack(double attackCoef) {
	hpCoef -= attackCoef * defenseCoef;
}

Aims* Unit::getAims() {
	return aims;
}

void Unit::appendAim(ActionParameter* actionParameter) {
	if (actionParameter->getAims() != aims && aims != nullptr) {
		aims->reduce();
		aimIndex = 0;
		aims = actionParameter->getAims();
		aims->up();
	} else if (aims == nullptr) {
		aimIndex = 0;
		aims = actionParameter->getAims();
		aims->up();
	}
}

void Unit::addAim(ActionParameter* actionParameter) {
	if (actionParameter->getAims() != aims && aims != nullptr) {
		aims->reduce();
	}
	aimIndex = 0;
	aims = actionParameter->getAims();
	aims->up();
}

void Unit::attack(Entity* entity) {
	if (this->team != entity->getTeam()) {
		entity->absorbAttack(attackCoef);
	}
}

void Unit::applyForce(double timeStep) {
	double coef = timeStep / mass;
	(*velocity) += (*acceleration) * coef;
	double velLenght = velocity->LengthSquared();
	if (velLenght > maxSpeed * maxSpeed) {
		velocity->Normalize();
		(*velocity) *= maxSpeed;
	} else if (velLenght > 0 && velLenght < minSpeed * minSpeed) {
		velocity->x_ = 0;
		velocity->y_ = 0;
		velocity->z_ = 0;
	}
}

int Unit::getType() {
	return UNIT;
}

void Unit::select() {
	Node* title = node->CreateChild("title");
	title->SetPosition(Vector3(0.0f, 1.2f, 0.0f));
	Text3D* titleText = title->CreateComponent<Text3D>();
	titleText->SetText("Entity");

	titleText->SetFont(font, 24);
	titleText->SetColor(Color::GREEN);
	titleText->SetAlignment(HA_CENTER, VA_CENTER);
	titleText->SetFaceCameraMode(FC_LOOKAT_MIXED);
	StaticModel* model = node->GetComponent<StaticModel>();

	model->SetMaterial(Game::get()->getCache()->GetResource<Urho3D::Material>("Materials/green.xml"));


	Node* planeNode = node->CreateChild("healthBar");
	planeNode->SetScale(Vector3(1, 1, 0.3f));
	planeNode->SetPosition(Vector3(0, 1.5f, 0));
	planeNode->Pitch(-90);

	StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
	planeObject->SetModel(Game::get()->getCache()->GetResource<Model>("Models/Plane.mdl"));
	planeObject->SetMaterial(Game::get()->getCache()->GetResource<Material>("Materials/red.xml"));
}

void Unit::unSelect() {
	Node* child = node->GetChild("title");
	if (child) {
		child->RemoveAllChildren();
		node->RemoveChild(child);
	}

	StaticModel* model = node->GetComponent<StaticModel>();
	model->SetMaterial(nullptr);//SetMaterial(Game::get()->getCache()->GetResource<Urho3D::Material>("Materials/green.xml"));
}
