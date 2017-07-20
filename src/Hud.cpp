#include "Hud.h"
#include "Game.h"
#include <regex>
#include <exprtk/exprtk.hpp>

void Hud::replaceVariables(XMLFile* xmlFile, int hudSizeId) {
	auto styleString = xmlFile->ToString();
	const char* chars = styleString.CString();

	std::string asStr(chars);

	exprtk::symbol_table<double> symbol_table;
	int varsSize = Game::get()->getDatabaseCache()->getHudVarsSize();
	for (int i = 0; i < varsSize; ++i) {//TODO to lepiej zrobiczapytaniem?
		db_hud_vars* var = Game::get()->getDatabaseCache()->getHudVar(i);
		if (var != nullptr && var->hud_size == hudSizeId) {
			symbol_table.add_variable(var->name.CString(), var->value);
		}
	}

	typedef exprtk::expression<double> expression_t;
	typedef exprtk::parser<double> parser_t;
	expression_t expression;
	expression.register_symbol_table(symbol_table);

	std::regex reg("\\{[^\\}]*\\}");

	std::regex_iterator<std::string::iterator> iterator(asStr.begin(), asStr.end(), reg);
	std::regex_iterator<std::string::iterator> rend;
	std::vector<double>* values = new vector<double>();

	while (iterator != rend) {
		std::string expression_string = iterator->str().substr(1, iterator->str().length() - 2);
		parser_t parser;
		parser.compile(expression_string, expression);
		double y = expression.value();
		values->push_back(y);
		++iterator;
	}

	for (auto var : (*values)) {
		asStr = std::regex_replace(asStr, reg, std::to_string((int)var), regex_constants::format_first_only);
	}

	xmlFile->FromString(asStr.c_str());
	delete values;
}

Hud::Hud() {
	buttons = new std::vector<HudElement*>();
	lists = new std::vector<HudElement*>();
	windows = new std::vector<Window*>();
	db_graph_settings* graphSettings = Game::get()->getDatabaseCache()->getGraphSettings(0);
	style = Game::get()->getCache()->GetResource<XMLFile>("UI/" + graphSettings->style);
	hudSize = Game::get()->getDatabaseCache()->getHudSize(graphSettings->hud_size);

	replaceVariables(style, graphSettings->hud_size);

	font = Game::get()->getCache()->GetResource<Font>("Fonts/Anonymous Pro.ttf");

	createMenu();
	createBuild();
	createTop();
	createUnits();
	createSelectedInfo();
	createMyDebugHud();
	createMiniMap();

	SharedPtr<Cursor> cursor(new Cursor(Game::get()->getContext()));
	cursor->SetStyleAuto(style);
	Game::get()->getUI()->SetCursor(cursor);

	cursor->SetPosition(Game::get()->getGraphics()->GetWidth() / 2, Game::get()->getGraphics()->GetHeight() / 2);
	selectedHudPanel = new SelectedHudPanel(style, hudSize, font, selectedInfoWindow);
}

Hud::~Hud() {
}

template <std::size_t SIZE>
void Hud::populateList(Font* font, DropDownList* dropDownList, std::array<String, SIZE> elements) {
	for (String mode : elements) {
		Text* text = new Text(Game::get()->getContext());
		text->SetText(mode);
		text->SetFont(font, 12);
		dropDownList->AddItem(text);
	}
}

void Hud::initDropDownList(DropDownList* dropDownList) {
	dropDownList->SetStyle("MyDropDown", style);

	menuWindow->AddChild(dropDownList);
}

void Hud::createMenu() {
	menuWindow = createWindow("MenuWindow");

	DropDownList* dropDownList = new DropDownList(Game::get()->getContext());

	std::array<String, 3> modes{{"Select","Build","Deploy"}};
	populateList(font, dropDownList, modes);

	initDropDownList(dropDownList);

	HudElement* hudElement = new HudElement(dropDownList);
	dropDownList->SetVar("HudElement", hudElement);
	lists->push_back(hudElement);
}

void Hud::createBuild() {
	buildWindow = createWindow("BuildWindow");
	createBuildingIcons();
}

void Hud::createUnits() {
	unitsWindow = createWindow("UnitsWindow");
	createUnitIcons();
}

void Hud::createBuildingIcons() {
	int size = Game::get()->getDatabaseCache()->getBuildingTypeSize();

	for (int i = 0; i < size; ++i) {
		db_building_type* buidling = Game::get()->getDatabaseCache()->getBuildingType(i);
		Texture2D* texture = Game::get()->getCache()->GetResource<Texture2D>("textures/hud/icon/" + buidling->icon);

		Sprite* sprite = createSprite(texture, hudSize->icon_size_x - hudSize->space_size_x, hudSize->icon_size_y - hudSize->space_size_y, style, "Sprite");
		Button* button = simpleButton(sprite, style, "Icon");

		HudElement* hudElement = new HudElement(button);
		hudElement->setBuildingType(BuildingType(i));
		button->SetVar("HudElement", hudElement);

		buttons->push_back(hudElement);
		buildWindow->AddChild(button);
	}
}

void Hud::createUnitIcons() {
	int size = Game::get()->getDatabaseCache()->getUnitTypeSize();
	ListView* scrollBar = unitsWindow->CreateChild<ListView>();
	
	scrollBar->SetStyleAuto(style);
	//scrollBar->SetLayout(LM_VERTICAL, 0, IntRect::ZERO);
	scrollBar->SetScrollBarsVisible(false, true);
	//scrollBar->SetIndentSpacing(hudSize->space_size_y);
	scrollBar->SetEnabled(true);
	scrollBar->SetHighlightMode(HM_ALWAYS);
	scrollBar->SetSelectOnClickEnd(false);

	//scrollBar->SetClearSelectionOnDefocus(false);
	for (int i = 0; i < size; ++i) {
		db_unit_type* unit = Game::get()->getDatabaseCache()->getUnitType(i);
		Texture2D* texture = Game::get()->getCache()->GetResource<Texture2D>("textures/hud/icon/" + unit->icon);

		Sprite* sprite = createSprite(texture, hudSize->icon_size_x - hudSize->space_size_x, hudSize->icon_size_y - hudSize->space_size_y, style, "Sprite");
		Button* button = simpleButton(sprite, style,  "Icon");

		HudElement* hudElement = new HudElement(button);
		hudElement->setUnitType(UnitType(i));
		button->SetVar("HudElement", hudElement);
		buttons->push_back(hudElement);
		scrollBar->AddItem(button);
		//scrollBar->AddChild(button);
	}
}

void Hud::createTop() {
	topWindow = createWindow("TopWindow");

	int size = Game::get()->getDatabaseCache()->getResourceSize();
	for (int i = 0; i < size; ++i) {
		db_resource* resource = Game::get()->getDatabaseCache()->getResource(i);
		Texture2D* texture = Game::get()->getCache()->GetResource<Texture2D>("textures/hud/icon/" + resource->icon);

		Sprite* sprite = createSprite(texture, (hudSize->icon_size_x - hudSize->space_size_x) / 2, (hudSize->icon_size_y - hudSize->space_size_y) / 2, style, "Sprite");
		Button* button = simpleButton(sprite, style, "TopButtons");

		topWindow->AddChild(button);
	}
}

void Hud::createSelectedInfo() {
	selectedInfoWindow = createWindow("SelectedInfoWindow");
}

void Hud::createMiniMap() {
	miniMapWindow = createWindow("MiniMapWindow");
}

void Hud::createDebugHud() {
	DebugHud* debugHud = Game::get()->getEngine()->CreateDebugHud();
	debugHud->SetDefaultStyle(style);
}

void Hud::createConsole() {
	Console* console = Game::get()->getEngine()->CreateConsole();
	console->SetDefaultStyle(style);
	console->GetBackground()->SetOpacity(0.8f);
}

void Hud::createMyDebugHud() {
	myDebugHud = createWindow("MyDebugHudWindow");
	
	fpsText = myDebugHud->CreateChild<Text>();
	fpsText->SetStyle("MyText", style);
}

void Hud::updateHud(Benchmark* benchmark, CameraManager* cameraManager) {
	Urho3D::String msg = "FPS: " + String(benchmark->getLastFPS());
	msg += "\navg FPS: " + String(benchmark->getAverageFPS());
	msg += "\nLoops: " + String(benchmark->getLoops());
	msg += "\nCamera: ";
	msg += "\n\t" + cameraManager->getInfo();

	fpsText->SetText(msg);
}

std::vector<HudElement*>* Hud::getButtonsToSubscribe() {
	return buttons;
}

std::vector<HudElement*>* Hud::getListsToSubscribe() {
	return lists;
}

std::vector<Window*>* Hud::getWindows() {
	return windows;
}

void Hud::updateState(ControlsState state) {
	switch (state) {
	case SELECT:
		buildWindow->SetVisible(false);
		unitsWindow->SetVisible(false);
		selectedInfoWindow->SetVisible(true);
		break;
	case BUILD:
		buildWindow->SetVisible(true);
		unitsWindow->SetVisible(false);
		selectedInfoWindow->SetVisible(false);
		break;
	case DEPLOY:
		buildWindow->SetVisible(false);
		unitsWindow->SetVisible(true);
		selectedInfoWindow->SetVisible(false);
		break;
	default: ;
	}
}

void Hud::updateSelected(SelectedInfo* selectedInfo) {//TODO raz stworzyc a sterowac widzialnsocia
	if (selectedInfo->hasChanged()) {
		selectedHudPanel->updateSelected(selectedInfo);
	}
}

Window* Hud::createWindow(const String& styleName) {
	Window* window = new Window(Game::get()->getContext());
	windows->push_back(window);
	window->SetStyle(styleName, style);
	Game::get()->getUI()->GetRoot()->AddChild(window);
	return window;
}
