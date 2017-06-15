#include "PlayersManager.h"


PlayersManager::PlayersManager() {
	allPlayers = new std::vector<Player*>();
	activePlayer = new Player(0, 0);
	allPlayers->push_back(activePlayer);
	Player* enemy = new Player(0, 1);
	allPlayers->push_back(enemy);
	teams[0] = new std::vector<Player*>();
	teams[1] = new std::vector<Player*>();
	teams[0]->push_back(activePlayer);
	teams[1]->push_back(enemy);
}


PlayersManager::~PlayersManager() {
}

Player* PlayersManager::getActivePlayer() {
	return activePlayer;
}

std::vector<Player*>* PlayersManager::getTeam(int i) {
	return teams[i];
}

void PlayersManager::changeActive(int i) {
	activePlayer = (*allPlayers)[i];
}
