#include "ChargeAim.h"
#include "Game.h"
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/StaticModel.h>

ChargeAim::ChargeAim(Urho3D::Vector3* begin, Urho3D::Vector3* end) {
	direction = new Urho3D::Vector3((*end) - (*begin));
	direction->y_ = 0;
	direction->Normalize();

	Game* game = Game::get();
	node = game->getScene()->CreateChild();
	node->SetPosition(((*end) + (*begin)) / 2);
	//node->SetRotation(Urho3D::Quaternion(0, 0, 180));

	Urho3D::StaticModel* model = node->CreateComponent<Urho3D::StaticModel>();
	model->SetModel(game->getCache()->GetResource<Urho3D::Model>("Models/arrow.mdl"));
	model->SetMaterial(Game::get()->getCache()->GetResource<Urho3D::Material>("Materials/red.xml"));

	distance = 50;
}

ChargeAim::ChargeAim(Urho3D::Vector3* _direction) {
	direction = new Urho3D::Vector3(*_direction);
	direction->y_ = 0;
	direction->Normalize();
	distance = 50;
}


ChargeAim::~ChargeAim() {
	delete direction;
}

Urho3D::Vector3* ChargeAim::getDirection(Unit* unit) {
	return new Vector3((*direction));
}

bool ChargeAim::ifReach(Unit* unit) {
	return false;
}

bool ChargeAim::expired() {
	return false;
}
