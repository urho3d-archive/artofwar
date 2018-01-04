#pragma once
#include "../AbstractWindowPanel.h"
#include "Game.h"
#include "ObjectEnums.h"
#include "SelectedHudElement.h"
#include "control/SelectedInfo.h"
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Text.h>


class SelectedHudPanel :public AbstractWindowPanel
{
public:
	SelectedHudPanel(Urho3D::XMLFile* _style);
	~SelectedHudPanel();	
	void update(SelectedInfo* selectedInfo);
	
	std::vector<Button*>& getButtonsSelectedToSubscribe();
	void prepareTexture(int sizeUnits, IntVector2 spriteSize, ObjectType type, std::vector<Texture2D*>& vector);
	int iconSize();
	void createRows();

private:
	void hide(int i);
	void createBody() override;
	short maxInRow = 31;
	short LINES_IN_SELECTION = 4;
	static String getIconName(ObjectType index, int i);
	std::vector<Button*> buttons;
	std::vector<Texture2D*> texturesUnits;
	std::vector<Texture2D*> texturesBuildings;
	std::vector<Texture2D*> texturesResources;
	SelectedHudElement** elements;
	UIElement **rows;
};

