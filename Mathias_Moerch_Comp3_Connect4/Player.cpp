#include "Player.h"

Player::Player(std::string _name, int _wins, int _losses, int _color, char _symbol) {
	name = _name;
	wins = _wins;
	losses = _losses;
	color = _color;
	symbol = _symbol;
}


Player::~Player() {
}