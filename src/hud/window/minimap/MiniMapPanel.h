#pragma once
#include "../AbstractWindowPanel.h"
#include <Urho3D/Resource/Image.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/Button.h>

#define MINI_MAP_BUTTON_NUMBER 4


class MiniMapPanel : public AbstractWindowPanel
{
public:
	MiniMapPanel(Urho3D::XMLFile* _style);
	~MiniMapPanel();
	void createEmpty(int parts);
	void update();
private:
	void createBody() override;
	Urho3D::Sprite* spr;
	Urho3D::Image* minimap;
	Urho3D::Texture2D* text;

	Urho3D::Button** buttons;

	unsigned* heightMap;
	int indexUpdate = 0;
	int indexPerUpdate;
};
