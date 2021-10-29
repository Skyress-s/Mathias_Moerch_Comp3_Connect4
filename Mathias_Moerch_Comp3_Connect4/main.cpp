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
			cout << a_board[j][a_board[0].size() - i - 1].item;
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
				content = '|';
			}
			cout << '[' << content << ']';
		}
		cout << endl;
		drawBoard(a_board);

		char ans = _getch();
		switch (tolower(ans)) {
		case 'd':
			if (currentChoice + 1 < width) {
				currentChoice++;
			}
			break;

		case 'a':
			if (currentChoice -1 >= 0) {
				currentChoice--;
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

bool isOutOfRange(int x, int y, int add, vector<vector<Tile>> a_board) {
	if (x + add > a_board.size() - 1 || x + add < 0) {
		return true;
	}
	else if (y + add > a_board[0].size() - 1 || y + add < 0) {
		return true;
	}
	else return false;
}

//int checkRay(int x, int y, vector<vector<Tile>> a_board, )

bool isWon(int x, int y, vector<vector<Tile>> a_board) {
	//gets the newest symbol placed
	char placedSym = a_board[x][y].item;


	//checks the / diagonal
	int points{1};
	int n{1};
	bool fin{ false };
	

	while (!fin) {
		if (isOutOfRange(x, y, n, a_board)) {
			break;
		}
		
		if (a_board[x + n][y + n].item == placedSym) {
			points++;
		}

		n++;
		
	}
	n = -1;
	while (!fin) {
		if (isOutOfRange(x, y, n, a_board)) {
			break;
		}

		if (a_board[x + n][y + n].item == placedSym) {
			points++;
		}
		n--;
	}
	
	

	cout << points << endl << endl;
	return true;
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
int castRay(position pos, position dir, vector<vector<Tile>> a_board) {
	char placedSym = a_board[pos.x][pos.y].item;
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
	return score;
}

bool isWon2(position pos, vector<vector<Tile>> a_board) {
	//start with checking diagonal rays
	//checking //
	int score = 1;
	score += castRay(pos, position(1, 1), a_board);
	score += castRay(pos, position(-1, -1), a_board);
	cout << " // : " << score << endl;
	if (score >= 4) return true;
	

	//checking \\ 
	score = 1;
	score += castRay(pos, position(-1, +1), a_board);
	score += castRay(pos, position(+1, -1), a_board);
	cout << " \\\\ : " << score << endl;
	if (score >= 4) return true;


	//checking --
	score = 1;
	score += castRay(pos, position(+1, 0), a_board);
	score += castRay(pos, position(-1, 0), a_board);
	cout << " -- : " << score << endl;
	if (score >= 4) return true;

	
	//checking ||
	score = 1;
	score += castRay(pos, position(0, +1), a_board);
	score += castRay(pos, position(0, -1), a_board);
	cout << " || : " << score << endl;
	if (score >= 4) return true;

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
/// animates the fall downwards and return the apropiate height value
/// </summary>
/// <param name="a_board"></param>
/// <param name="a_dp"></param>
/// <returns>int value of the height where the item should be placed</returns>
int animateFall(vector<vector<Tile>> a_board, int a_dp) {
	int maxHeight = a_board[0].size() - 1;
	////starts witha check to see if the edgecase where for when the collum is filled up
	//if (a_board[a_dp][maxHeight - 1].item != '*') {

	//}


	int currentHeight = maxHeight;
	bool foundBottom{ false };
	while (!foundBottom) {
		//removes the previous item above
		if (currentHeight < maxHeight) {
			a_board[a_dp][currentHeight + 1].item = '*';
		}

		//checks if you have reached a occupied tile
		if (a_board[a_dp][currentHeight].item != '*') {
			return currentHeight + 1;
		}


		a_board[a_dp][currentHeight].item = activePlayer;
		currentHeight--;
		system("cls");
		drawBoard(a_board);
		Sleep(200);

		//checks if you have reaced the bottom
		if (currentHeight <= 0) {
			return 0;
		}

		
		
		


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

void mainGameloop(vector<vector<Tile>> a_board) {
	bool finishedGame{ false };
	while (!finishedGame) {
		//checks if proppoint is valid, if not, return
		int dropPoint = playerChooseSlot(a_board);
		if (!isDropPointValid(a_board, dropPoint)) 
			break;

		//int h = animateFall(a_board, dropPoint);
		int h = calcFallPos(a_board, dropPoint);
		a_board[dropPoint][h].item = activePlayer;

		if (isWon2(position(dropPoint, h), a_board)) {
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