#pragma once
#include "SettingsForm.h"
#include "hud/window/main_menu/MainMenuDetailsPanel.h"
#include <Urho3D/Resource/Localization.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/CheckBox.h>
#include <Urho3D/UI/DropDownList.h>


#define SETTINGS_ROWS_NUMBER 9

class MainMenuSettingsPanel : public MainMenuDetailsPanel
{
public:
	MainMenuSettingsPanel(Urho3D::XMLFile* _style, Urho3D::String _title);
	~MainMenuSettingsPanel();
	void setValues(int graphID);
private:
	void createBody() override;
	void populateLabels(int index, Urho3D::String name);
	void popualateForm(SettingsForm* form);
	void HandleChangeSettings(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	void HandleSaveSettings(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

	Urho3D::BorderImage* rows[SETTINGS_ROWS_NUMBER];
	Urho3D::Button* save;

	Urho3D::DropDownList* settings;
	Urho3D::DropDownList* resolution;
	Urho3D::CheckBox* fullScreen;
	Urho3D::DropDownList* maxFps;
	Urho3D::DropDownList* minFps;
	Urho3D::CheckBox* vSync;
	Urho3D::DropDownList* textureQuality;
	Urho3D::CheckBox* shadow;
	Urho3D::DropDownList* hudSize;

	SettingsForm * data;
};
