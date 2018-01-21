#include "MenuPanel.h"
#include "Game.h"
#include "database/DatabaseCache.h"
#include "GameState.h"


MenuPanel::MenuPanel() : AbstractWindowPanel() {
	styleName = "MenuWindow";
	visibleAt.insert(GameState::RUNNING);
	visibleAt.insert(GameState::PAUSE);
}


MenuPanel::~MenuPanel() = default;

void MenuPanel::removeInfo() {
	text->SetVisible(false);
	text2->SetVisible(false);
}

Urho3D::String MenuPanel::stringFrom(db_unit* dbUnit, std::vector<db_cost*>* costs) {
	String msg = dbUnit->name + "\n";
	for (db_cost* cost : (*costs)) {
		msg += cost->resourceName + " - " + String(cost->value) + "\n";
	}
	return msg;
}

Urho3D::String MenuPanel::stringFrom(db_building* dbBuilding, std::vector<db_cost*>* costs) {
	String msg = dbBuilding->name + "\n";
	for (db_cost* cost : (*costs)) {
		msg += cost->resourceName + " - " + String(cost->value) + "\n";
	}
	return msg;
}

void MenuPanel::setInfo(HudElement* hudElement) {
	String s = "";
	ObjectType type = hudElement->getType();
	const short id = hudElement->getId();
	switch (type) {

	case UNIT:
		{
		db_unit* dbUnit = Game::get()->getDatabaseCache()->getUnit(id);
		std::vector<db_cost*>* costs = Game::get()->getDatabaseCache()->getCostForUnit(id);
		s = stringFrom(dbUnit, costs);
		}
		break;
	case BUILDING:
		{
		db_building* dbBuilding = Game::get()->getDatabaseCache()->getBuilding(id);
		std::vector<db_cost*>* costs = Game::get()->getDatabaseCache()->getCostForBuilding(id);
		s = stringFrom(dbBuilding, costs);
		}
		break;
	default: ;
	}
	text->SetVisible(true);
	text->SetText(s);

}

void MenuPanel::updateSelected(SelectedInfo* selectedInfo) {
	if(selectedInfo->getAllNumber()==1) {
		text->SetVisible(true);
		std::vector<SelectedInfoType*>& infoTypes = selectedInfo->getSelecteType();
		for (auto & infoType : infoTypes) {
			std::vector<Physical*>& data = infoType->getData();
			if (!data.empty()) {
				Physical* physical = data.at(0);

				text->SetText(physical->toMultiLineString());
				break;
			}
		}
	}else {
		text->SetVisible(false);
	}

	text2->SetText(selectedInfo->getMessage());
	text2->SetVisible(true);
}

void MenuPanel::createBody() {
	text = window->CreateChild<Urho3D::Text>();
	text->SetStyle("MyText");
	text->SetText("");

	text2 = window->CreateChild<Urho3D::Text>();
	text2->SetStyle("MyText");
	text2->SetText("");
}
