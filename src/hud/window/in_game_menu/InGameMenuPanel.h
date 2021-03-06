#pragma once
#include "hud/window/AbstractWindowPanel.h"
#include "middle/AbstractMiddlePanel.h"
#include <Urho3D/UI/Button.h>


class InGameMenuPanel : public AbstractWindowPanel
{
public:
	InGameMenuPanel(Urho3D::XMLFile* _style);
	~InGameMenuPanel();
	void setVisible(bool enable) override;
	Urho3D::Button* getSaveButton();

private:
	void toggle();
	void action(short id);
	void close();
	void createBody() override;
	void HandleButtonClick(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	void HandleToggle(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	bool menuVisibility = false;
	Urho3D::Button* toggleButton;
	AbstractMiddlePanel** addionalPanels;
};
