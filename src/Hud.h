#pragma once
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/CheckBox.h>
#include <Urho3D/UI/LineEdit.h>
#include <Urho3D/UI/DropDownList.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/ToolTip.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Drawable.h>
#include <Urho3D/UI/ListView.h>
#include "camera/CameraManager.h"
#include "Benchmark.h"

#include "HudElement.h"
#include "db_strcut.h"
#include "ControlsState.h"
#include "SelectedInfo.h"
#include "SelectedHudPanel.h"
#include "BuildPanel.h"
#include "UnitsPanel.h"
#include "DebugPanel.h"
#include "MenuPanel.h"
#include "TopPanel.h"
#include "MiniMapPanel.h"
#include "OrdersPanel.h"
#include "QueuePanel.h"

using namespace Urho3D;

class Hud
{
public:
	Hud();
	~Hud();

	void update(Benchmark* benchmark, CameraManager* cameraManager);
	void update(int unitsNumber);
	void updateSelected(SelectedInfo* selectedInfo);

	void hoverOnIcon(HudElement* hudElement);
	void hoverOffIcon(HudElement* hudElement);

	std::vector<Button*>* getButtonsSelectedToSubscribe();
	std::vector<HudElement*>* getButtonsOrdersToSubscribe();
	std::vector<HudElement*>* getButtonsBuildToSubscribe();
	std::vector<HudElement*>* getButtonsUnitsToSubscribe();
	std::vector<Window*>* getWindows();
private:
	void replaceVariables(XMLFile* xmlFile, int hudSizeId);
	void createDebugHud();
	void createConsole();
	void createCursor();

	SelectedHudPanel* selectedHudPanel;
	BuildPanel* buildPanel;
	UnitsPanel* unitsPanel;
	DebugPanel* debugPanel;
	TopPanel* topPanel;
	MiniMapPanel* miniMapPanel;
	MenuPanel* menuPanel;
	OrdersPanel* ordersPanel;
	QueuePanel* queuePanel;

	std::vector<Window*>* windows;
	db_graph_settings* graphSettings;
	XMLFile* style;
};
