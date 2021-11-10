#pragma once
#include <string>
#include <iostream>

class Player {
public:
	Player(std::string, int, int, int, char);
	~Player();
	std::string name{};
	int wins{};
	int losses{};
	int color{};
	char symbol{ 'A' };

	void printInfo() {
		std::cout << "name    : " << name << std::endl;
		std::cout << "wins    : " << wins << std::endl;
		std::cout << "losses  : " << losses << std::endl;
	}
private:

};

