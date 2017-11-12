#include "LoadingPanel.h"
#include "Game.h"
#include <Urho3D/UI/UI.h>


LoadingPanel::LoadingPanel(Urho3D::XMLFile* _style) : AbstractWindowPanel(_style) {
	styleName = "LoadingWindow";
}


LoadingPanel::~LoadingPanel() {
}

void LoadingPanel::createBody() {

	background = window->CreateChild<Urho3D::BorderImage>();
	background->SetStyle("Background", style);
	background->SetVisible(true);
	background->SetOpacity(0.5);


	bar = window->CreateChild<Urho3D::ProgressBar>();
	bar->SetStyle("LargeProgressBar", style);
	bar->SetRange(1);
	bar->SetValue(0);
	bar->SetVisible(true);

}

void LoadingPanel::update(float progres) {
	bar->SetValue(progres);
}
