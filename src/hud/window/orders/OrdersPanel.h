#pragma once
#include "../AbstractWindowPanel.h"
#include "../../HudElement.h"
#include <vector>
#include "control/SelectedInfo.h"

class OrdersPanel :public AbstractWindowPanel
{
public:
	OrdersPanel();
	~OrdersPanel();
	void show(SelectedInfo* selectedInfo);
	std::vector<HudElement*>& getButtons();
private:
	void createBody() override;
	std::vector<HudElement*> buttons;
};

