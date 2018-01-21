#include "BuildPanel.h"
#include "../../HudElement.h"
#include "../../UiUtils.h"
#include <Urho3D/UI/ListView.h>
#include "objects/building/Building.h"
#include "database/DatabaseCache.h"
#include <Urho3D/Resource/ResourceCache.h>
#include "utils.h"
#include "GameState.h"
#include "player/PlayersManager.h"


BuildPanel::BuildPanel(): AbstractWindowPanel() {
	styleName = "BuildWindow";

	visibleAt.insert(GameState::RUNNING);
	visibleAt.insert(GameState::PAUSE);
}


BuildPanel::~BuildPanel() {
	clear_vector(buttons);
}

std::vector<HudElement*>& BuildPanel::getButtons() {
	return buttons;
}

void BuildPanel::show() {
	setVisible(true);
	int nation = Game::get()->getPlayersManager()->getActivePlayer()->getNation();
	for (auto button : buttons) {

		db_building* building = Game::get()->getDatabaseCache()->getBuilding(button->getId());
		if (building->nation == nation) {
			button->getUIElement()->SetVisible(true);
		} else {
			button->getUIElement()->SetVisible(false);
		}
	}
}

void BuildPanel::createBody() {
	int size = Game::get()->getDatabaseCache()->getBuildingSize();

	buttons.reserve(size);
	ListView* panel = window->CreateChild<ListView>();
	panel->SetStyle("MyListView");

	for (int i = 0; i < size; ++i) {
		db_building* building = Game::get()->getDatabaseCache()->getBuilding(i);
		if (building) {
			Texture2D* texture = Game::get()->getCache()->GetResource<Texture2D>("textures/hud/icon/" + building->icon);

			MySprite* sprite = createSprite(texture, "Sprite");
			Button* button = simpleButton(sprite, "Icon");

			HudElement* hudElement = new HudElement(button);
			hudElement->setId(i, ObjectType::BUILDING);

			button->SetVar("HudElement", hudElement);
			buttons.push_back(hudElement);
			panel->AddItem(button);
		}
	}
	BorderImage* element = panel->CreateChild<BorderImage>();
	element->SetStyle("EditorDivider");
	panel->AddItem(element);

}
