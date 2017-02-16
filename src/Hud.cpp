#include "Hud.h"



Hud::Hud(Context* context, UI* _ui, ResourceCache* _cache, Graphics* _graphics) {
	ui = _ui;
	cache = _cache;
	window = new Window(context);
	ui->GetRoot()->AddChild(window);

	// Set Window size and layout settings
	window->SetMinWidth(384);
	window->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
	window->SetAlignment(HA_RIGHT, VA_TOP);
	window->SetName("Window");

	// Create a CheckBox
	CheckBox* checkBox = new CheckBox(context);
	checkBox->SetName("CheckBox");

	// Create a Button
	Button* button = new Button(context);
	button->SetName("Button");
	button->SetMinHeight(24);

	// Create a LineEdit
	LineEdit* lineEdit = new LineEdit(context);
	lineEdit->SetName("LineEdit");
	lineEdit->SetMinHeight(24);

	// Add controls to Window
	window->AddChild(checkBox);
	window->AddChild(button);
	window->AddChild(lineEdit);

	// Apply previously set default style
	checkBox->SetStyleAuto();
	button->SetStyleAuto();
	lineEdit->SetStyleAuto();

	XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
	SharedPtr<Cursor> cursor(new Cursor(context));
	cursor->SetStyleAuto(style);
	ui->SetCursor(cursor);

	cursor->SetPosition(_graphics->GetWidth() / 2, _graphics->GetHeight() / 2);
}


Hud::~Hud() {}

void Hud::createStaticHud(String msg) {
	Text* instructionText = ui->GetRoot()->CreateChild<Text>();
	instructionText->SetText(msg);
	instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
	instructionText->SetTextAlignment(HA_CENTER);
	instructionText->SetHorizontalAlignment(HA_LEFT);
	instructionText->SetVerticalAlignment(VA_TOP);
	instructionText->SetPosition(0, 0);
}
