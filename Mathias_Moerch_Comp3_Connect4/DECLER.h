#pragma once


#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <string>
#include <cwchar>
#include <Windows.h>

#include "termcolor.hpp"
#include "Position.h"
#include "Tile.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

char activePlayer{};

char  p1 = 'X';
string p1Name{ "PLAYER ONE" };

char  p2 = 'O';
string p2Name{ "PLAYER TWO" };

char EMPTY_PIECE = '*';
int turn{};

const string playersFile = "Players.txt";

/// <summary>
/// function for clearing the cin buffer for errors and emptying it of left over data
/// Gotten from Johannes TA
/// </summary>
void ClearCin();


void mainMenu();



// AI --------------------------------------

vector<int> minimax(vector<vector<Tile>> a_board, Position pos, int depth, bool maximizing);

vector<Position> scoreOfTile(Position pos, vector<vector<Tile>> a_board);

int evalLine(vector<int> arr, char player);

int evalRowOfFour(vector<int> row, char player);

// GAME LOGIC ------------------------------

/// <summary>
/// Can choose between the options vector, 
/// the title will always print above the choices, space to accept answer where arrow is,
/// W and S to go up and down
/// </summary>
/// <param name="options"></param>
/// <param name="title"></param>
/// <returns> the index of the answer chosen </returns>
int Choice(vector<string> options, string title);

void InitGame();

void inputNames();

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

int playerChooseSlot(vector<vector<Tile>> a_board, bool a_activeAI);

void assignWinnerTilesBoard(vector<vector<Tile>>& a_board, vector<Position> poses);

