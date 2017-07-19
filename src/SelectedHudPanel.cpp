#include "SelectedHudPanel.h"

SelectedHudPanel::SelectedHudPanel(Urho3D::XMLFile* _style, db_hud_size* _hudSize, Font* _font, Window* _window) {
	style = _style;
	hudSize = _hudSize;
	font = _font;
	window = _window;
	buttons = new Button**[OBJECT_TYPE_NUMBER];
	label = new Text(Game::get()->getContext());
	window->AddChild(label);
	//int entityTypeSize = Game::get()->getDatabaseCache()->getEmtityTypeSize();
	int unitTypeSize = Game::get()->getDatabaseCache()->getUnitTypeSize();
	int buildingTypeSize = Game::get()->getDatabaseCache()->getBuildingTypeSize();
	int resourceSize = Game::get()->getDatabaseCache()->getResourceSize();

	//buttons[ObjectType::ENTITY] = new Urho3D::Button*[entityTypeSize];
	buttons[ObjectType::UNIT] = new Urho3D::Button*[unitTypeSize];
	buttons[ObjectType::BUILDING] = new Urho3D::Button*[buildingTypeSize];
	buttons[ObjectType::RESOURCE] = new Urho3D::Button*[resourceSize];

	//createButton(entityTypeSize, ENTITY);
	createButton(unitTypeSize, UNIT);
	createButton(buildingTypeSize, BUILDING);
	createButton(resourceSize, RESOURCE);

}

void SelectedHudPanel::createButton(int size, ObjectType index) {
	for (int i = 0; i < size; ++i) {
		String name = getName(index, i);

		Texture2D* texture = Game::get()->getCache()->GetResource<Texture2D>("textures/hud/icon/" + name);

		Sprite* sprite = createSprite(texture, hudSize->icon_size_x - hudSize->space_size_x, hudSize->icon_size_y - hudSize->space_size_y);
		buttons[index][i] = simpleButton(style, sprite, "Icon");
		Text * text = new Text(Game::get()->getContext());
		text->SetName("Text");
		buttons[index][i]->AddChild(text);
		buttons[index][i]->SetVisible(false);
		window->AddChild(buttons[index][i]);
	}
}

SelectedHudPanel::~SelectedHudPanel() {
}

void SelectedHudPanel::hide() {
	for (int i = 0; i < OBJECT_TYPE_NUMBER; ++i) {
		int size = getSize(ObjectType(i));
		for (int j = 0; j < size; ++j) {
			buttons[i][j]->SetVisible(false);
		}
	}
}

String SelectedHudPanel::getName(ObjectType index, int i) {
	DatabaseCache* dbCache = Game::get()->getDatabaseCache();
	switch (index) {
	case ENTITY:
		return "mock.png";
	case UNIT:
		return dbCache->getUnitType(i)->icon;
	case BUILDING:
		return dbCache->getBuildingType(i)->icon;
	case RESOURCE:
		return dbCache->getResource(i)->icon;
	default:
		return "mock.png";;
	}
}

int SelectedHudPanel::getSize(ObjectType type) {
	DatabaseCache* dbCache = Game::get()->getDatabaseCache();
	switch (type) {
	case ENTITY:
		return 0;
	case UNIT:
		return dbCache->getUnitTypeSize();
	case BUILDING:
		return dbCache->getBuildingTypeSize();
	case RESOURCE:
		return dbCache->getResourceSize();
	default:
		return 0;
	}
}

void SelectedHudPanel::updateSelected(SelectedInfo* selectedInfo) {
	label->SetText((*selectedInfo->getMessage()));
	label->SetFont(font, 12);

	hide();
	ObjectType type = selectedInfo->getSelectedType();
	int size = getSize(type);
	String** lines = selectedInfo->getLines();
	for (int i = 0; i < size; ++i) {
		if ((lines[i]) != nullptr) {
			Text * text = (Text*)buttons[selectedInfo->getSelectedType()][i]->GetChild("Text", false);

			text->SetText((*lines[i]));
			text->SetFont(font, 12);
			buttons[selectedInfo->getSelectedType()][i]->SetVisible(true);	
		}

	}
}
