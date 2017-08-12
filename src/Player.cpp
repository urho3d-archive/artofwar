#include "Player.h"
#include "Game.h"


Player::Player(int nationId, int team) {
	dbNation = Game::get()->getDatabaseCache()->getNation(nationId);
	resources = new Resources();
	this->team = team;
}

Player::~Player() {

}

int Player::getNation() {
	return dbNation->id;
}

int Player::getTeam() {
	return team;
}
