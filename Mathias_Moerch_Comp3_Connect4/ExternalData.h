#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Player.h"
using std::cout;
using std::endl;
using std::cin;
using std::vector;
using std::string;


vector<Player> loadFromLog(string filepath);

void writeToLog(vector<Player> a_players, string filepath);

void AddOrModifyPlayer(string filepath, Player a_newP);

