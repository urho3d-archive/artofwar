#include "QueuePanel.h"
#include "Game.h"
#include "objects/building/Building.h"
#include <Urho3D/Resource/ResourceCache.h>
#include "database/DatabaseCache.h"
#include "Urho3D/Graphics/Texture2D.h"
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Input/InputEvents.h>
#include "GameState.h"
#include "objects/NamesCache.h"


QueuePanel::QueuePanel(Urho3D::XMLFile* _style) : AbstractWindowPanel(_style) {
	styleName = "QueueWindow";
	elements = new QueueHudElement*[MAX_ICON_SELECTION];
	visibleAt.insert(GameState::RUNNING);
	visibleAt.insert(GameState::PAUSE);
}

QueuePanel::~QueuePanel() {
	for (int i = 0; i < MAX_ICON_SELECTION; ++i) {
		delete elements[i];
	}
	delete[] elements;
}

void QueuePanel::update(QueueManager* queue, short& j) {
	const short size = queue->getSize();
	for (int i = 0; i < size; ++i) {
		QueueElement* element = queue->getAt(i);
		elements[j]->show();
		String name = getIconName(element->getType(), element->getSubtype());
		Texture2D* texture = Game::get()->getCache()->GetResource<Texture2D>("textures/hud/icon/" + name);
		elements[j]->setText(String(element->getAmount()) + "/" + String(element->getMaxCapacity()));
		elements[j]->setTexture(texture);
		elements[j]->setProgress(element->getProgress());
		elements[j]->setData(element);

		++j;
	}
}

void QueuePanel::show(SelectedInfo* selectedInfo) {
	//TODO tu moga byc problemy przy duzej ilosci
	setVisible(true);
	update(selectedInfo);
}

void QueuePanel::update(SelectedInfo* selectedInfo) {
	//TODO wykonuje sie nawet je�li sie nic nie zmieni�o
	short j = 0;
	if (window->IsVisible()) {
		std::vector<SelectedInfoType*>& infoTypes = selectedInfo->getSelectedTypes();

		for (auto & infoType : infoTypes) {
			std::vector<Physical*>& data = infoType->getData();
			for (Physical* physical : data) {
				//TODO przeniesc kolejke do Physical
				Building* building = dynamic_cast<Building*>(physical);
				update(building->getQueue(), j);
			}
		}
	}
	hideElements(j);
}

void QueuePanel::hideElements(int from) {
	for (int i = from; i < MAX_ICON_SELECTION; ++i) {
		elements[i]->hide();
	}
}

void QueuePanel::createBody() {
	for (int i = 0; i < MAX_ICON_SELECTION; ++i) {
		elements[i] = new QueueHudElement(style);
		window->AddChild(elements[i]->getButton());
		SubscribeToEvent(elements[i]->getButton(), E_CLICK, URHO3D_HANDLER(QueuePanel, HandleReduce));
	}
}

void QueuePanel::HandleReduce(StringHash eventType, VariantMap& eventData) {
	UIElement* element = static_cast<UIElement*>(eventData[Urho3D::UIMouseClick::P_ELEMENT].GetVoidPtr());
	QueueHudElement* qHudElement = static_cast<QueueHudElement *>(element->GetVar("QueueHudElement").GetVoidPtr());

	int button = eventData[UIMouseClick::P_BUTTON].GetInt();
	if (button == MOUSEB_LEFT) {
		qHudElement->reduce(1);
	} else {
		qHudElement->reduce(10);
	}
}
