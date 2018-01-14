#include "MainMenuNewGamePanel.h"
#include "Game.h"
#include "database/DatabaseCache.h"


MainMenuNewGamePanel::
MainMenuNewGamePanel(Urho3D::XMLFile* _style, Urho3D::String _title): MainMenuDetailsPanel(_style, _title) {
	bodyStyle = "MainMenuNewGameMock";
}

void MainMenuNewGamePanel::createBody() {
	MainMenuDetailsPanel::createBody();
	for (int i = 0; i < NEW_GAME_ROWS_NUMBER; ++i) {
		rows[i] = body->CreateChild<Urho3D::BorderImage>();
		rows[i]->SetStyle("MainMenuNewGameRow");
	}
	Urho3D::Localization* l10n = Game::get()->getLocalization();
	myLine.init(style, l10n);
	enemyLine.init(style, l10n);

	populateLabels(l10n, 0, "player");
	populateLabels(l10n, 1, "enemy");
	populateLabels(l10n, 2, "map_name");
	populateLabels(l10n, 3, "difficulty");
	populateLabels(l10n, 4, "game_speed");

	myLine.populateTeams(rows[0]);
	enemyLine.populateTeams(rows[1]);

	Urho3D::DropDownList* map = createDropDownList(rows[2], "MainMenuNewGameDropDownList", style);
	int mapsSize = Game::get()->getDatabaseCache()->getMapSize();
	for (int i = 0; i < mapsSize; ++i) {
		addTextItem(map, Game::get()->getDatabaseCache()->getMap(i)->name, style);
	}

	Urho3D::DropDownList* difficulty = createDropDownList(rows[3], "MainMenuNewGameDropDownList", style);
	addTextItem(difficulty, l10n->Get("easy"), style);
	addTextItem(difficulty, l10n->Get("normal"), style);
	addTextItem(difficulty, l10n->Get("hard"), style);

	Urho3D::DropDownList* gameSpeed = createDropDownList(rows[4], "MainMenuNewGameDropDownList", style);
	addTextItem(gameSpeed, l10n->Get("slow"), style);
	addTextItem(gameSpeed, l10n->Get("normal"), style);
	addTextItem(gameSpeed, l10n->Get("fast"), style);

	proceed = body->CreateChild<Urho3D::Button>();
	proceed->SetStyle("MainMenuNewGameButton");
	Urho3D::Text* text = proceed->CreateChild<Urho3D::Text>();
	text->SetStyle("MainMenuNewGameButtonText", style);
	text->SetText(l10n->Get("start"));
}

void MainMenuNewGamePanel::populateLabels(Urho3D::Localization* l10n, int index, Urho3D::String name) {
	Urho3D::Text* text = rows[index]->CreateChild<Urho3D::Text>();
	text->SetStyle("MainMenuNewGameButtonLabel", style);
	text->SetText(l10n->Get(name));
}


MainMenuNewGamePanel::~MainMenuNewGamePanel() {
}
