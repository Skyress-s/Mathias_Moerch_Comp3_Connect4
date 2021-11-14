#pragma once


#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <string>
#include <cwchar>
#include <Windows.h>
#include <mutex>

#include "Player.h"
#include "termcolor.hpp"
#include "Position.h"
#include "Tile.h"
#include "ExternalData.h"
#include "Visual.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

// a global drop point (x value for where to drop the piece in the game
// i have this in global space to ensure that the xpos en saved between turns in the main game loop, 
//int globalDP{ 3 };

//global empty piece
char EMPTY_PIECE = '*';

//global space for the active player
//This is a bad practive to have things in global scope, but i feel it is worth it in this case becouse nearly all the functions need
//a refrence to the activePlayer.symbol of color
Player* activePlayer{};

//constant for the filepath for the players log .txt file
const string playersFile = "Players.txt";





void ClearCin();


void mainMenu();



// AI --------------------------------------

void AnimateAIDecition(std::vector<std::vector<Tile>>& a_board, int dropPoint, int& currentDP);

vector<int> minimax(vector<vector<Tile>> a_board, Position pos,  int depth, int alpha, int beta, bool maximizing);

vector<Position> scoreOfTile(Position pos, vector<vector<Tile>> a_board);

int evalLine(vector<int> arr, char player);

int evalRowOfFour(vector<int> row, char player);

// GAME LOGIC ------------------------------

int Choice(vector<string> options, string title);
int colorChoice( std::pair<string, int> title);


void InitGame(bool);

void inputName(Player& a_player);

void AnimateAIDecition(std::vector<std::vector<Tile>>& a_board, int dropPoint, int& currentDP);

void mainGameloop(vector<vector<Tile>> a_board, bool a_activeAI);

int scoreOfBoard(vector<vector<Tile>> a_board, char player);

bool isBoardFull(vector<vector<Tile>> a_board);

void toggleActivePlayer();

int calcFallPos(vector<vector<Tile>> a_board, int a_dp);

void castRay(Position pos, Position dir, vector<vector<Tile>> a_board, vector<Position>& score);

bool isOutOfRange(Position pos, vector<vector<Tile>> a_board);

bool isDropPointValid(vector<vector<Tile>> a_board, int a_dropPoint);

// VISUAL ----------------------------------


void addDotsToConsole(int dots, float duration);

void animateFall(vector<vector<Tile>> a_board, Position pos, int stepDuration);

void drawBoard(vector<vector<Tile>> a_board);

int playerChooseSlot(vector<vector<Tile>> a_board, bool a_activeAI, int& currentDP);

void AIShowSlot(vector<vector<Tile>> a_board, bool empty, int& currentDP);

void assignWinnerTilesBoard(vector<vector<Tile>>& a_board, vector<Position> poses);

void addColor(int col);

void stats();

void options();

void mainMenu();

int Choice(vector<string> options, string title);

int colorChoice(std::pair<string, int> title);

void inputPlayerSymbol(Player& a_player);