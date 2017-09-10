#include "UnitFactory.h"


UnitFactory::UnitFactory(): EntityFactory() {
	units = new std::vector<Unit *>();
	units->reserve(DEFAULT_VECTOR_SIZE);
	states = new StateManager();
}

UnitFactory::~UnitFactory() {
	units->clear();
	delete units;
	delete states;
}

std::vector<Unit*>* UnitFactory::create(unsigned number, int id, Vector3* center, SpacingType spacing, int player) {
	Game* game = Game::get();
	db_unit* dbUnit = game->getDatabaseCache()->getUnit(id);

	double space = getSpecSize(spacing);
	int produced = 0;
	int y = 0;
	int xMax = number / sqrt(number);
	double sideSize = xMax * space / 2;

	Model* model3d = game->getCache()->GetResource<Model>("Models/" + dbUnit->model);
	Material* material = Game::get()->getCache()->GetResource<Urho3D::Material>("Materials/" + dbUnit->texture);

	while (produced < number) {
		for (int x = 0; x < xMax; ++x) {
			Vector3* position = new Vector3(x * space + center->x_ - sideSize, 0 + center->y_,
			                                y * space + center->z_ - sideSize);
			Node* node = game->getScene()->CreateChild();
			node->SetPosition(*position);

			StaticModel* model = node->CreateComponent<StaticModel>();
			model->SetModel(model3d);
			model->SetMaterial(material);

			Unit* newUnit = new Unit(position, node);
			newUnit->populate(dbUnit, states);
			newUnit->setPlayer(player);
			newUnit->setTeam(player);//TODO ustawic team
			units->push_back(newUnit);
			++produced;
			if (produced >= number) { break; }
		}
		++y;
	}
	return units;
}
