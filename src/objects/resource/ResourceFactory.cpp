#include "ResourceFactory.h"
#include "Game.h"
#include "objects/building/Building.h"
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>


ResourceFactory::ResourceFactory() {
	resources = new std::vector<ResourceEntity *>();
	resources->reserve(DEFAULT_VECTOR_SIZE);
}


ResourceFactory::~ResourceFactory() {
	resources->clear();
	delete resources;
}

std::vector<ResourceEntity*>* ResourceFactory::create(unsigned number, int id, Vector3* center, SpacingType spacing) {
	db_resource* dbResource = Game::get()->getDatabaseCache()->getResource(id);

	double space = getSpecSize(spacing) * spaceCoef;
	String modelName = "Models/" + dbResource->model;
	int produced = 0;
	int y = 0;
	int xMax = number / sqrt(number);
	double sideSize = xMax * space / 2;
	while (produced < number) {
		for (int x = 0; x < xMax; ++x) {
			Node* node = Game::get()->getScene()->CreateChild();
			Vector3* position = new Vector3(x * space + center->x_ - sideSize, 0 + center->y_,
			                                y * space + center->z_ - sideSize);
			node->SetPosition((*position));
			node->Scale(dbResource->scale);
			StaticModel* model = node->CreateComponent<StaticModel>();
			model->SetModel(Game::get()->getCache()->GetResource<Model>(modelName));
			for (int i = 0; i < dbResource->texture.Size(); ++i) {
				model->SetMaterial(i, Game::get()->getCache()->GetResource<Material>("Materials/" + dbResource->texture[i]));
			}

			ResourceEntity* entity = new ResourceEntity(position, node);
			entity->populate(dbResource);
			resources->push_back(entity);
			++produced;
			if (produced >= number) { break; }
		}
		++y;
	}
	return resources;
}
