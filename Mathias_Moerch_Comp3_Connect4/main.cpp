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
char const p2 = 'O';

#define EMPTY_PIECE '*'



int turn{};

struct Tile {
	char item{};
	bool winnerTile{ false };
	

};



void drawBoard(vector<vector<Tile>> a_board) {
	cout << " ---------------------" << endl;
	for (int i = 0; i < a_board[0].size(); i++) {
		cout << "| ";
		for (int j = 0; j < a_board.size(); j++) {
			Tile tile = a_board[j][a_board[0].size() - i - 1];
			if (tile.winnerTile) {
				cout << termcolor::magenta;
			}
			else if (tile.item == p1) {
				cout << termcolor::bright_cyan;
			}
			else if (tile.item == p2) {
				cout << termcolor::bright_red;
			}
			
			cout << a_board[j][a_board[0].size() - i - 1].item;
			cout << termcolor::reset;

			if (a_board.size() - 1 > j) {
				cout << "  ";
			}
		}
		cout << " |" << endl;
	}
	cout << " ---------------------" << endl;
}


int playerChooseSlot(vector<vector<Tile>> a_board) {
	int currentChoice{ 0 };
	bool finished{ false };
	while (!finished) {
		system("cls");

		//draws the choice bar
		cout << " ";
		for (int i = 0; i < a_board.size(); i++) {
			char content{ ' ' };
			if (i == currentChoice) {
				content = activePlayer;
				if (activePlayer == p1) {
					cout << termcolor::bright_cyan;
				}
				else if (activePlayer == p2) {
					cout << termcolor::bright_red;
				}
			}
			cout << '[' << content << ']';
			cout << termcolor::reset;
		}
		cout << endl;
		drawBoard(a_board);

		char ans = _getch();
		switch (tolower(ans)) {
		case 'd':
			if (currentChoice + 1 < a_board.size()) {
				currentChoice++;
			}
			else {
				currentChoice = 0;
			}
			break;

		case 'a':
			if (currentChoice -1 >= 0) {
				currentChoice--;
			}
			else {
				currentChoice = a_board.size() - 1;
			}
			break;

		case ' ':
			finished = true;
			break;

		default:
			break;
		}
	}
	return currentChoice;
}

bool isDropPointValid(vector<vector<Tile>> a_board, int a_dropPoint) {
	int maxHeight = a_board[0].size();
	char potentialBlockedPos = a_board[a_dropPoint][maxHeight - 1].item;
	if (potentialBlockedPos == '*') {
		return true;
		system("pause");
	}
	else {
		return false;
	}
}

struct position {
	int x{};
	int y{};

	position(int x1, int y1) {
		x = x1;
		y = y1;
	}
};

bool isOutOfRange(position pos, vector<vector<Tile>> a_board) {
	if (pos.x > a_board.size()-1 || pos.x < 0) {
		return true;
	}
	else if (pos.y > a_board[0].size()-1 || pos.y < 0) {
		return true;
	}
	else if (a_board[pos.x][pos.y].item == '*') {
		return true;
	}
	return false;
}
void castRay(position pos, position dir, vector<vector<Tile>> a_board, vector<position> &score) {
	char placedSym = a_board[pos.x][pos.y].item;
	//vector<position> score{};
	
	int i{ 1 };
	while (true) {
		if (isOutOfRange(position(pos.x + dir.x * i, pos.y + dir.y * i), a_board))
			break;


		if (placedSym == a_board[pos.x + dir.x * i][pos.y + dir.y * i].item) { // shooting ray until it hits a opposite player or empty field
			score.push_back(position(pos.x + dir.x * i, pos.y + dir.y * i));
		}
		else { // if it hits something other than the same symbol as the active player, break
			break;
		}
		i++;
	}
	return;
}

vector<position> scoreOfTile(position pos, vector<vector<Tile>> a_board) {
	//start with checking diagonal rays
	//checking //
	vector<position> score{};



	castRay(pos, position(1, 1), a_board, score);
	castRay(pos, position(-1, -1), a_board, score);
	//cout << " // : " << score.size() << endl;
	score.push_back(pos);
	if (score.size() >= 4) return score;
		
	//checking \\ 
	score.clear();
	castRay(pos, position(-1, +1), a_board, score);
	castRay(pos, position(+1, -1), a_board, score);
	//cout << " \\\\ : " << score.size() << endl;
	score.push_back(pos);
	if (score.size() >= 4) return score;

	//checking --
	score.clear();
	castRay(pos, position(+1, 0), a_board, score);
	castRay(pos, position(-1, 0), a_board, score);
	//cout << " -- : " << score.size() << endl;
	score.push_back(pos);
	if (score.size() >= 4) return score;

	//checking ||
	score.clear();
	castRay(pos, position(0, +1), a_board, score);
	castRay(pos, position(0, -1), a_board, score);
	//cout << " || : " << score.size() << endl;
	score.push_back(pos);
	if (score.size() >= 4) return score;


	return score;
}

int scoreOfBoard(vector<vector<Tile>> a_board, char player) {
	//checking -- rows (x)

	for (int i = 0; i < a_board[0].size(); i++) {
		int score{};
		int inRow{};
		int emptyRow{};
		for (int j = 0; j < a_board.size(); j++) {


			bool hitOpp{ false };
			if (a_board[j][i].item == player) {
				inRow++;
			}
			else if (a_board[j][i].item == EMPTY_PIECE) {
				emptyRow++;
			}
			else { // hit enemy
				
				if (inRow == 2 && emptyRow >= 2) {
					score += 2;
				}
				else if (inRow == 3 && emptyRow>= 1) {
					score += 5;
				}
				else if (inRow == 4) {
					score += 10;
				}

				emptyRow = 0;
				inRow = 0;

				hitOpp = true;
			}
			if (hitOpp == false && j == a_board.size()-1) { // last tile and has not added score

				if (inRow == 2 && emptyRow >= 2) {
					score += 2;
				}
				else if (inRow == 3 && emptyRow >= 1) {
					score += 5;
				}
				else if (inRow == 4) {
					score += 10;
				}
			}
		}

		inRow = 0;
		emptyRow = 0;
		//opponent for loop
		for (int j = 0; j < a_board.size(); j++) {
			bool hitOpp{ false };
			if (a_board[j][i].item != player && a_board[j][i].item != EMPTY_PIECE) {
				inRow++;
			}
			else if (a_board[j][i].item == EMPTY_PIECE) {
				emptyRow++;
			}
			else { // hit enemy

				if (inRow == 3 && emptyRow >= 1) {
					score -= 400;
				}
				

				emptyRow = 0;
				inRow = 0;

				hitOpp = true;
			}
			if (hitOpp == false && j == a_board.size() - 1) { // last tile and has not added score

				if (inRow == 3 && emptyRow >= 1) {
					score -= 4000;
				}
				
			}
		}

		cout << score << endl;

	}
		system("pause");
	

	return 0;
}

int calcFallPos(vector<vector<Tile>> a_board, int a_dp) {

	int currentHeight = a_board[0].size() - 1;
	while (true) {
		if (currentHeight <= 0) {
			return 0;
		}

		if (a_board[a_dp][currentHeight - 1].item != '*') {
			return currentHeight;
		}

		currentHeight--;
	}
}

/// <summary>
/// animates the fall downwards for the newest piece
/// </summary>
/// <param name="a_board"></param>
/// <param name="a_dp"></param>
/// <returns></returns>
void animateFall(vector<vector<Tile>> a_board, position pos, int stepDuration) {
	int currentHeight = a_board[0].size() - 1;

	while (currentHeight > pos.y) {
		a_board[pos.x][currentHeight].item = activePlayer;
		if (currentHeight < a_board[0].size() - 1) {
			a_board[pos.x][currentHeight + 1].item = '*';
		}
		
		system("cls");
		drawBoard(a_board);
		Sleep(stepDuration);
		currentHeight--;
	}

}

/// <summary>
/// toggles what player is the active player
/// </summary>
void toggleActivePlayer() {
	if (activePlayer == p1) {
		activePlayer = p2;
	}
	else {
		activePlayer = p1;
	}
}

void addDotsToConsole(int dots, float duration) {
	for (int i = 0; i < dots; i++) {
		Sleep(duration);
		cout << '.';
	}
	Sleep(duration);
}

void assignWinnerTilesBoard(vector<vector<Tile>> &a_board, vector<position> poses) {
	for (int i = 0; i < poses.size(); i++) {
		a_board[ poses[i].x ][ poses[i].y ].winnerTile = true;
	}
}

bool isBoardFull(vector<vector<Tile>> a_board) {
	for (int i = 0; i < a_board.size(); i++) {
		for (int j = 0; j < a_board[0].size(); j++) {
			if (a_board[i][j].item == '*') {
				return false;
			}
		}
	}
	return true;
}



int minimax(vector<vector<Tile>> a_board, position pos, int depth, bool maximizing) {

	drawBoard(a_board);
	system("pause");

	int score = scoreOfTile(pos, a_board).size();

	if (depth == 4) {
		score = 0;
	}

	if (depth == 0 or score >= 4) {
		if (score >= 4 && maximizing == true) {
			return  +100000;
		}
		else if (score >= 4 && maximizing == false) {
			return  -100000;
		}
		else {
			if (maximizing) {
				return score;
			}
			else {
				return 0;
			}
		}
	}
	
	//cout << "DEPTH IS " << depth << endl;
	//cout << "maximizing : " << maximizing << endl;
	//system("pause");
	int maxEval{};
	if (maximizing) {
		int maxEval = -10000000;
		for (int i = 0; i < a_board.size(); i++) {
			//vector<vector<Tile>> c_board = a_board;
			if (isDropPointValid(a_board, i) == false) {
				continue;
			}

			int n = calcFallPos(a_board, i); // gets the fallposition
			char temp = a_board[i][n].item;
			a_board[i][n].item = p2; // the new gamestate

			int eval = minimax(a_board, position(i, n), depth - 1, false);
			maxEval = max(maxEval, eval); 

			a_board[i][n].item = temp;
		}
		return maxEval;
	}
	else {
		int maxEval = 10000000;
		for (int i = 0; i < a_board.size(); i++) {
			//vector<vector<Tile>> c_board = a_board;
			if (isDropPointValid(a_board, i) == false) {
				continue;
			}

			int n = calcFallPos(a_board, i); // gets the fallposition
			char temp = a_board[i][n].item;
			a_board[i][n].item = p1; // the new gamestate
			
			int eval = minimax(a_board, position(i, n), depth - 1, true);
			maxEval = min(maxEval, eval);

			a_board[i][n].item = temp;
		}
		return maxEval;
	}
}


void mainGameloop(vector<vector<Tile>> a_board) {
	bool finishedGame{ false };
	while (!finishedGame) {
		turn++;
		system("cls");
		if (activePlayer == false) {
			
			for (int i = 0; i < a_board.size(); i++) {
				vector<vector<Tile>> tempBoard = a_board;

				int height = calcFallPos(tempBoard, i);
				tempBoard[i][height].item = p2;

				//system("cls");
				int score = minimax(tempBoard, position(0, 0), 4, false);
				//scores.push_back(score);

				cout << "drop point position " << i + 1 << "  - Gives score : " << score << endl;
			}
			system("pause");

			
		}



		//checks if proppoint is valid, if not, return
		int dropPoint = playerChooseSlot(a_board);
		if (!isDropPointValid(a_board, dropPoint)) {
			cout << "Invalid placement, please choose again";
			addDotsToConsole(3, 500.f);
			continue;
		}

		//int h = animateFall(a_board, dropPoint);
		int h = calcFallPos(a_board, dropPoint);
		animateFall(a_board, position(dropPoint, h), 20);
		a_board[dropPoint][h].item = activePlayer;

		vector<position> score = scoreOfTile(position(dropPoint, h), a_board);
		cout << score.size() << endl;
		//system("pause");


		if (score.size() >= 4) {
			assignWinnerTilesBoard(a_board, score);
			system("cls");
			drawBoard(a_board);
			system("pause");
			cout << activePlayer << " has won!" << endl;
			finishedGame = true;
		}

		if (isBoardFull(a_board)) {
			system("cls");
			drawBoard(a_board);
			cout << "Board is " << termcolor::bright_blue << "FULL. Its a draw" << termcolor::reset << endl;
			system("pause");
			finishedGame = true;
		}


		scoreOfBoard(a_board, activePlayer);
		toggleActivePlayer();
	}
}



int main() {
	
	vector<vector<Tile>> board(7, vector<Tile>(6, Tile{'*', false}));
	
	activePlayer = 'X';


	mainGameloop(board);
	return 0 ;

}