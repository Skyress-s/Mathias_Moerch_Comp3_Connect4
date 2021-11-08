#pragma once

#include <iostream>
#include <vector>
#include "termcolor.hpp"
#include <conio.h>
#include <string>
#include <cwchar>
#include <Windows.h>


using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

char activePlayer{};

char const p1 = 'X';
string p1Name{ "PLAYER ONE" };

char const p2 = 'O';
string p2Name{ "PLAYER TWO" };

#define EMPTY_PIECE '*'
int turn{};


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


/// <summary>
/// clear cin func from johannes TA 
/// </summary>
void ClearCin();
