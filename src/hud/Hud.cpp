#include "Hud.h"
#include "Game.h"
#include <regex>
#include <exprtk/exprtk.hpp>
#include "ButtonUtils.h"

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
	double resX = (double)graphSettings->res_x;
	double resY = (double)graphSettings->res_y;
	symbol_table.add_variable("resX", resX);
	symbol_table.add_variable("resY", resY);

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

void Hud::createCursor() {
	SharedPtr<Cursor> cursor(new Cursor(Game::get()->getContext()));
	cursor->SetStyleAuto(style);
	Game::get()->getUI()->SetCursor(cursor);
	cursor->SetPosition(Game::get()->getGraphics()->GetWidth() / 2, Game::get()->getGraphics()->GetHeight() / 2);
}

Hud::Hud() {
	windows = new std::vector<Window*>();
	graphSettings = Game::get()->getDatabaseCache()->getGraphSettings(0);

	style = Game::get()->getCache()->GetResource<XMLFile>("UI/" + graphSettings->style);
	replaceVariables(style, graphSettings->hud_size);
	Game::get()->getUI()->GetRoot()->SetDefaultStyle(style);

	createConsole();
	createDebugHud();
	createCursor();

	int nation = Game::get()->getPlayersManager()->getActivePlayer()->getNation();

	selectedHudPanel = new SelectedHudPanel(style);
	buildPanel = new BuildPanel(style, nation);
	unitsPanel = new UnitsPanel(style, nation);
	debugPanel = new DebugPanel(style);
	topPanel = new TopPanel(style);
	miniMapPanel = new MiniMapPanel(style);
	menuPanel = new MenuPanel(style);
	ordersPanel = new OrdersPanel(style);
	queuePanel = new QueuePanel(style);

	windows->push_back(menuPanel->createWindow());
	windows->push_back(buildPanel->createWindow());
	windows->push_back(unitsPanel->createWindow());
	windows->push_back(miniMapPanel->createWindow());
	windows->push_back(debugPanel->createWindow());
	windows->push_back(topPanel->createWindow());
	windows->push_back(selectedHudPanel->createWindow());
	windows->push_back(ordersPanel->createWindow());
	windows->push_back(queuePanel->createWindow());

	buildPanel->show();
}

Hud::~Hud() {
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

void Hud::update(Benchmark* benchmark, CameraManager* cameraManager) {
	Urho3D::String msg = "FPS: " + String(benchmark->getLastFPS());
	msg += "\navg FPS: " + String(benchmark->getAverageFPS());
	msg += "\nLoops: " + String(benchmark->getLoops());
	msg += "\nCamera: \n\t" + (*cameraManager->getInfo());

	debugPanel->setText(msg);

	topPanel->update(Game::get()->getPlayersManager()->getActivePlayer()->getResources());
}

void Hud::update(int unitsNumber) {
	topPanel->update(unitsNumber);
}

std::vector<HudElement*>* Hud::getButtonsBuildToSubscribe() {
	return buildPanel->getButtons();
}

std::vector<HudElement*>* Hud::getButtonsUnitsToSubscribe() {
	return unitsPanel->getButtons();
}

std::vector<Window*>* Hud::getWindows() {
	return windows;
}

void Hud::updateSelected(SelectedInfo* selectedInfo) {//TODO raz stworzyc a sterowac widzialnsocia
	if (selectedInfo->hasChanged()) {
		selectedHudPanel->update(selectedInfo);
		menuPanel->updateSelected(selectedInfo);
		switch (selectedInfo->getSelectedType()) {

		case UNIT:
			ordersPanel->show(selectedInfo);
			buildPanel->setVisible(false);
			unitsPanel->setVisible(false);
			queuePanel->setVisible(false);
			break;
		case BUILDING:
			ordersPanel->setVisible(false);
			buildPanel->setVisible(false);
			unitsPanel->show(selectedInfo);
			queuePanel->show(selectedInfo);
			break;
		default:
			ordersPanel->setVisible(false);
			buildPanel->show();
			unitsPanel->setVisible(false);
			queuePanel->setVisible(false);
		}
	} else {
		queuePanel->update(selectedInfo);
	}
}

std::vector<Button*>* Hud::getButtonsSelectedToSubscribe() {
	return selectedHudPanel->getButtonsSelectedToSubscribe();
}

void Hud::hoverOnIcon(HudElement* hudElement) {
	menuPanel->setInfo(hudElement);
}

void Hud::hoverOffIcon(HudElement* hudElement) {
	menuPanel->removeInfo();
}

std::vector<HudElement*>* Hud::getButtonsOrdersToSubscribe() {
	return ordersPanel->getButtons();
}