#include "UnitFactory.h"
#include "Unit.h"
#include <vector>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Graphics/Model.h>
#include "EntityFactory.h"
#include "LinkComponent.h"
#include "SimulationObjectManager.h"
#include "Game.h"

UnitFactory::UnitFactory(): EntityFactory() {

}

UnitFactory::~UnitFactory() {
}


std::vector<Unit*>* UnitFactory::createUnits() {
	int startSize = -(size / 2);
	int endSize = size + startSize;

	std::vector<Unit*>* units = new std::vector<Unit *>();
	units->reserve(size * size);
	Game * game = Game::getInstance();
	Font* font = game->getCache()->GetResource<Font>("Fonts/Anonymous Pro.ttf");
	for (int y = startSize; y < endSize; ++y) {
		for (int x = startSize; x < endSize; ++x) {
			Vector3 *position = new Vector3(x * space, 0, y * space);
			Node* node = game->getScene()->CreateChild("Box");
			node->SetPosition(*position);

			StaticModel* boxObject = node->CreateComponent<StaticModel>();
			boxObject->SetModel(game->getCache()->GetResource<Model>("Models/Cube.mdl"));

			Unit* newUnit = new Unit(position, node, font);
			units->push_back(newUnit);
		}
	}
	return units;
}
