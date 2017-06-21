#include "Hud.h"
#include "Game.h"


Hud::Hud() {
	buttons = new std::vector<HudElement*>();
	lists = new std::vector<HudElement*>();
	db_graph_settings* graphSettings = Game::get()->getDatabaseCache()->getGraphSettings(0);
	style = Game::get()->getCache()->GetResource<XMLFile>("UI/" + graphSettings->style);
	hudSize = Game::get()->getDatabaseCache()->getHudSize(graphSettings->hud_size);//TODO z settings to wziac
	font = Game::get()->getCache()->GetResource<Font>("Fonts/Anonymous Pro.ttf");

	createMenu();
	createBuild();
	createTop();
	createUnits();
	createSelectedInfo();
	createMyDebugHud();

	SharedPtr<Cursor> cursor(new Cursor(Game::get()->getContext()));
	cursor->SetStyleAuto(style);
	Game::get()->getUI()->SetCursor(cursor);

	cursor->SetPosition(Game::get()->getGraphics()->GetWidth() / 2, Game::get()->getGraphics()->GetHeight() / 2);

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
	dropDownList->SetFixedHeight(hudSize->icon_size_x / 2);
	dropDownList->SetFixedWidth(2 * hudSize->icon_size_x);
	dropDownList->SetResizePopup(true);

	dropDownList->SetStyleAuto(style);
	menuWindow->AddChild(dropDownList);
}

void Hud::createMenu() {
	menuWindow = new Window(Game::get()->getContext());
	Game::get()->getUI()->GetRoot()->AddChild(menuWindow);
	Texture2D* wood = Game::get()->getCache()->GetResource<Texture2D>("textures/wood.png");

	menuWindow->SetFixedWidth(2 * (hudSize->icon_size_x + hudSize->space_size_x));
	menuWindow->SetFixedHeight(2 * (hudSize->icon_size_y + hudSize->space_size_y));
	menuWindow->SetLayout(LM_VERTICAL, hudSize->space_size_x, IntRect(hudSize->space_size_x, hudSize->space_size_y, hudSize->space_size_x, hudSize->space_size_y));
	menuWindow->SetAlignment(HA_LEFT, VA_BOTTOM);
	menuWindow->SetName("Window");
	menuWindow->SetTexture(wood);
	menuWindow->SetTiled(true);

	DropDownList* dropDownList = new DropDownList(Game::get()->getContext());

	std::array<String, 3> modes{{"Select","Build","Deploy"}};
	populateList(font, dropDownList, modes);

	initDropDownList(dropDownList);

	HudElement* hudElement = new HudElement(dropDownList);
	dropDownList->SetVar("HudElement", hudElement);
	lists->push_back(hudElement);
}

void Hud::createBuild() {
	buildWindow = new Window(Game::get()->getContext());
	Game::get()->getUI()->GetRoot()->AddChild(buildWindow);
	Texture2D* wood = Game::get()->getCache()->GetResource<Texture2D>("textures/wood.png");
	buildWindow->SetMinWidth(512);
	buildWindow->SetMinHeight(hudSize->icon_size_y + 2 * hudSize->space_size_y);
	buildWindow->SetLayout(LM_HORIZONTAL, hudSize->space_size_x, IntRect(hudSize->space_size_x, hudSize->space_size_y, hudSize->space_size_x, hudSize->space_size_y));
	buildWindow->SetAlignment(HA_LEFT, VA_BOTTOM);
	buildWindow->SetPosition(2 * (hudSize->icon_size_x + hudSize->space_size_x), 0);
	buildWindow->SetTexture(wood);
	buildWindow->SetTiled(true);
	buildWindow->SetVisible(false);
	int typesSize = Game::get()->getDatabaseCache()->getBuildingTypeSize();

	createBuildingIcons();
}

void Hud::createUnits() {
	unitsWindow = new Window(Game::get()->getContext());
	Game::get()->getUI()->GetRoot()->AddChild(unitsWindow);
	Texture2D* wood = Game::get()->getCache()->GetResource<Texture2D>("textures/wood.png");
	unitsWindow->SetMinWidth(512);
	unitsWindow->SetMinHeight(hudSize->icon_size_y + 2 * hudSize->space_size_y);
	unitsWindow->SetLayout(LM_HORIZONTAL, hudSize->space_size_x, IntRect(hudSize->space_size_x, hudSize->space_size_y, hudSize->space_size_x, hudSize->space_size_y));
	unitsWindow->SetAlignment(HA_LEFT, VA_BOTTOM);
	unitsWindow->SetPosition(2 * (hudSize->icon_size_x + hudSize->space_size_x), 0);
	unitsWindow->SetTexture(wood);
	unitsWindow->SetTiled(true);
	unitsWindow->SetVisible(false);

	std::array<UnitType, 5> units{{WARRIOR, PIKEMAN, CAVALRY, ARCHER ,WORKER}};
	createUnitIcons(units);
}

Button* Hud::simpleButton(Urho3D::Sprite* sprite, int sizeX, int sizeY) {
	Button* button = new Button(Game::get()->getContext());
	button->SetStyleAuto(style);
	button->SetFixedSize(sizeX, sizeY);
	button->AddChild(sprite);
	return button;
}

Sprite* Hud::createSprite(Texture2D* texture, int sizeX, int sizeY) {
	Urho3D::Sprite* sprite = new Sprite(Game::get()->getContext());

	sprite->SetTexture(texture);
	int textureWidth = texture->GetWidth();
	int textureHeight = texture->GetHeight();
	double scaleX = (sizeX) / (double)textureWidth;
	double scaleY = (sizeY) / (double)textureHeight;
	if (scaleX < scaleY) {
		sprite->SetScale(scaleX);
	} else {
		sprite->SetScale(scaleY);
	}

	sprite->SetSize(textureWidth, textureHeight);
	sprite->SetHotSpot(textureWidth / 2, textureHeight / 2);
	sprite->SetAlignment(HA_CENTER, VA_CENTER);
	sprite->SetOpacity(0.9f);
	return sprite;
}


void Hud::createBuildingIcons() {
	int size = Game::get()->getDatabaseCache()->getBuildingTypeSize();

	for (int i = 0; i < size; ++i) {
		db_building_type* buidling = Game::get()->getDatabaseCache()->getBuildingType(i);
		Texture2D* texture = Game::get()->getCache()->GetResource<Texture2D>("textures/hud/icon/" + buidling->icon);

		Sprite* sprite = createSprite(texture, hudSize->icon_size_x - hudSize->space_size_x, hudSize->icon_size_y - hudSize->space_size_y);
		Button* button = simpleButton(sprite, hudSize->icon_size_x, hudSize->icon_size_y);

		HudElement* hudElement = new HudElement(button);
		hudElement->setBuildingType(BuildingType(i));
		button->SetVar("HudElement", hudElement);

		buttons->push_back(hudElement);
		buildWindow->AddChild(button);
	}
}

template <std::size_t SIZE>
void Hud::createUnitIcons(std::array<UnitType, SIZE> units) {
	for (UnitType type : units) {
		db_unit_type* unit = Game::get()->getDatabaseCache()->getUnitType(type);

		Texture2D* texture = Game::get()->getCache()->GetResource<Texture2D>("textures/hud/icon/" + unit->icon);

		Sprite* sprite = createSprite(texture, hudSize->icon_size_x - hudSize->space_size_x, hudSize->icon_size_y - hudSize->space_size_y);
		Button* button = simpleButton(sprite, hudSize->icon_size_x, hudSize->icon_size_y);

		HudElement* hudElement = new HudElement(button);
		hudElement->setUnitType(type);
		button->SetVar("HudElement", hudElement);
		buttons->push_back(hudElement);
		unitsWindow->AddChild(button);
	}
}

void Hud::createTop() {
	topWindow = new Window(Game::get()->getContext());
	Game::get()->getUI()->GetRoot()->AddChild(topWindow);
	Texture2D* wood = Game::get()->getCache()->GetResource<Texture2D>("textures/wood.png");

	topWindow->SetMinWidth(512);
	topWindow->SetMinHeight((hudSize->icon_size_y + 2 * hudSize->space_size_y) / 2);//TODO sprawdzic ten rozmiar czy to ma sens
	topWindow->SetLayout(LM_HORIZONTAL, hudSize->space_size_x, IntRect(hudSize->space_size_x, hudSize->space_size_y, hudSize->space_size_x, hudSize->space_size_y));
	topWindow->SetAlignment(HA_RIGHT, VA_TOP);
	topWindow->SetName("Window");
	topWindow->SetTexture(wood);
	topWindow->SetTiled(true);
	int size = Game::get()->getDatabaseCache()->getResourceSize();

	for (int i = 0; i < size; ++i) {
		db_resource* resource = Game::get()->getDatabaseCache()->getResource(i);
		Texture2D* texture = Game::get()->getCache()->GetResource<Texture2D>("textures/hud/icon/" + resource->icon);

		Sprite* sprite = createSprite(texture, (hudSize->icon_size_x - hudSize->space_size_x) / 2, (hudSize->icon_size_y - hudSize->space_size_y) / 2);

		Button* button = simpleButton(sprite, hudSize->icon_size_x * 2, hudSize->icon_size_y / 2);

		topWindow->AddChild(button);
	}
}

void Hud::createSelectedInfo() {
	selectedInfoWindow = new Window(Game::get()->getContext());
	Game::get()->getUI()->GetRoot()->AddChild(selectedInfoWindow);
	Texture2D* wood = Game::get()->getCache()->GetResource<Texture2D>("textures/wood.png");

	selectedInfoWindow->SetMinWidth(512);
	selectedInfoWindow->SetMinHeight(hudSize->icon_size_y + 2 * hudSize->space_size_y);
	selectedInfoWindow->SetLayout(LM_HORIZONTAL, hudSize->space_size_x, IntRect(hudSize->space_size_x, hudSize->space_size_y, hudSize->space_size_x, hudSize->space_size_y));
	selectedInfoWindow->SetAlignment(HA_CENTER, VA_BOTTOM);
	selectedInfoWindow->SetName("Window");
	selectedInfoWindow->SetTexture(wood);
	selectedInfoWindow->SetTiled(true);

}

void Hud::createStaticHud(String msg) {
	Text* instructionText = Game::get()->getUI()->GetRoot()->CreateChild<Text>();
	instructionText->SetText(msg);
	instructionText->SetFont(font, 12);
	instructionText->SetTextAlignment(HA_CENTER);
	instructionText->SetHorizontalAlignment(HA_LEFT);
	instructionText->SetVerticalAlignment(VA_TOP);
	instructionText->SetPosition(0, 0);
}

void Hud::createLogo() {
	Texture2D* logoTexture = Game::get()->getCache()->GetResource<Texture2D>("textures/minimap.png");
	if (!logoTexture) {
		return;
	}
	Urho3D::Sprite* logoSprite_ = Game::get()->getUI()->GetRoot()->CreateChild<Sprite>();

	logoSprite_->SetTexture(logoTexture);

	int textureWidth = logoTexture->GetWidth();
	int textureHeight = logoTexture->GetHeight();

	logoSprite_->SetScale(256.0f / textureWidth);
	logoSprite_->SetSize(textureWidth, textureHeight);
	logoSprite_->SetHotSpot(textureWidth, textureHeight);
	logoSprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
	logoSprite_->SetOpacity(0.9f);
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
	fpsText = Game::get()->getUI()->GetRoot()->CreateChild<Text>();
	fpsText->SetFont(font, 12);
	fpsText->SetTextAlignment(HA_LEFT);
	fpsText->SetHorizontalAlignment(HA_LEFT);
	fpsText->SetVerticalAlignment(VA_TOP);
	fpsText->SetPosition(0, 20);
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
	//SubscribeToEvent(hudElement->getUIElement(), E_CLICK, URHO3D_HANDLER(Main, HandleUIButtton));
	return buttons;
}

std::vector<HudElement*>* Hud::getListsToSubscribe() {
	return lists;
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

void Hud::updateSelected(SelectedInfo* selectedInfo) {
	if (selectedInfo->hasChanged()) {
		selectedInfoWindow->RemoveAllChildren();
		Text* text = new Text(Game::get()->getContext());
		text->SetText((*selectedInfo->getMessage()));
		text->SetFont(font, 12);
		selectedInfoWindow->AddChild(text);
		String** lines = selectedInfo->getLines();
		for (int i = 0; i < SELECTED_INFO_SIZE; ++i) {
			if ((lines[i]) != nullptr) {
				Text* text = new Text(Game::get()->getContext());
				text->SetText((*lines[i]));
				text->SetFont(font, 12);

				Button* button = new Button(Game::get()->getContext());
				button->SetStyleAuto(style);

				button->AddChild(text);
				button->SetFixedSize(hudSize->icon_size_x, hudSize->icon_size_y);
				selectedInfoWindow->AddChild(button);
			}
		}
	}
}
