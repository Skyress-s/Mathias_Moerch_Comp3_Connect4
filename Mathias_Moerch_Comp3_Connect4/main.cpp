#include <iostream>
#include <vector>
#include "termcolor.hpp"
#include <conio.h>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


int const width{ 7 };
int const height{ 6 };
char activePlayer{};
char const p1 = 'X';
char const p2 = 'O';


struct Tile {
	char item{};
	char color{ 'w' };
	

};



void drawBoard(vector<vector<Tile>> a_board) {
	cout << " ---------------------" << endl;
	for (int i = 0; i < a_board[0].size(); i++) {
		cout << "| ";
		for (int j = 0; j < a_board.size(); j++) {
			char item = a_board[j][a_board[0].size() - i - 1].item;
			if (item == p1) {
				cout << termcolor::bright_cyan;
			}
			else if (item == p2) {
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
		for (int i = 0; i < width; i++) {
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
			if (currentChoice + 1 < width) {
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
	/*char placedSym = a_board[pos.x][pos.y].item;
	int score{};
	position rayPos = pos;
	int i{ 1 };
	while (true) {
		if (isOutOfRange(position(pos.x + dir.x * i, pos.y + dir.y * i), a_board))
			break;

		if (placedSym == a_board[pos.x + dir.x * i][pos.y + dir.y * i].item) {
			score++;
		}
		i++;
	}
	return score;*/

	char placedSym = a_board[pos.x][pos.y].item;
	//vector<position> score{};
	
	int i{ 1 };
	while (true) {
		if (isOutOfRange(position(pos.x + dir.x * i, pos.y + dir.y * i), a_board))
			break;

		if (placedSym == a_board[pos.x + dir.x * i][pos.y + dir.y * i].item) { // shooting ray until it hits a opposite player or empty field
			score.push_back(position(pos.x + dir.x * i, pos.y + dir.y * i));
		}
		i++;
	}
	return;
}

bool isWon2(position pos, vector<vector<Tile>> a_board) {
	//start with checking diagonal rays
	//checking //
	vector<position> score{};
	castRay(pos, position(1, 1), a_board, score);
	castRay(pos, position(-1, -1), a_board, score);
	cout << " // : " << score.size() << endl;
	if (score.size() + 1 >= 4) return true;
	

	////checking \\ 
	//score = 1;
	//score += castRay(pos, position(-1, +1), a_board);
	//score += castRay(pos, position(+1, -1), a_board);
	//cout << " \\\\ : " << score << endl;
	//if (score >= 4) return true;


	////checking --
	//score = 1;
	//score += castRay(pos, position(+1, 0), a_board);
	//score += castRay(pos, position(-1, 0), a_board);
	//cout << " -- : " << score << endl;
	//if (score >= 4) return true;

	//
	////checking ||
	//score = 1;
	//score += castRay(pos, position(0, +1), a_board);
	//score += castRay(pos, position(0, -1), a_board);
	//cout << " || : " << score << endl;
	//if (score >= 4) return true;

	return false;

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

void mainGameloop(vector<vector<Tile>> a_board) {
	bool finishedGame{ false };
	while (!finishedGame) {
		//checks if proppoint is valid, if not, return
		int dropPoint = playerChooseSlot(a_board);
		if (!isDropPointValid(a_board, dropPoint)) {
			cout << "Invalid placement, please choose again";
			addDotsToConsole(3, 500.f);
			continue;
		}

		//int h = animateFall(a_board, dropPoint);
		int h = calcFallPos(a_board, dropPoint);
		animateFall(a_board, position(dropPoint, h), 150);
		a_board[dropPoint][h].item = activePlayer;

		if (isWon2(position(dropPoint, h), a_board)) {
			system("cls");
			drawBoard(a_board);
			cout << activePlayer << " has won!!!!!!!!!!!!" << endl;
			system("pause");
			finishedGame = true;
		}

		toggleActivePlayer();
	}
}



int main() {
	
	vector<vector<Tile>> board(width, vector<Tile>(height, Tile{'*', 'w'}));


	activePlayer = 'X';


	mainGameloop(board);
	return 0 ;

}