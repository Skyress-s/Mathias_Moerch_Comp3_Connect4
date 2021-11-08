#include "DECLER.h"



int main() {

	InitGame();


	return 0;

}

void ClearCin() { 
	std::cin.clear();    //Clears eventual errors from buffer
	std::cin.ignore(32767, '\n');    //clears the buffer if anything is there
}


// AI / --------------------------------------

/// <summary>
/// for the score i used some of the scruture from this connect four alg https://github.com/KeithGalli/Connect4-Python/blob/503c0b4807001e7ea43a039cb234a4e55c4b226c/connect4_with_ai.py#L85
/// Implementation of the minimax alg, choses based of the heuristic value of the board and end of game scores
/// </summary>
/// <param name="a_board"></param>
/// <param name="pos"></param>
/// <param name="depth"></param>
/// <param name="maximizing"></param>
/// <returns>return vector<int>(2), 0 is score, 1 is path</returns>
vector<int> minimax(vector<vector<Tile>> a_board, Position pos, int depth, bool maximizing) { // first is score, second is path

	/*drawBoard(a_board);
	system("pause");*/

	int score = scoreOfTile(pos, a_board).size();
	if (depth == 0 || score >= 4) {
		if (score >= 4 && maximizing == true) {
			/*drawBoard(a_board);
			cout << "WIN!" << endl;
			system("pause");*/
			return  vector<int>{-1000, pos.x};
		}
		else if (score >= 4 && maximizing == false) {
			/*drawBoard(a_board);
			cout << "LOSS!" << endl;
			system("pause");*/
			return  vector<int>{1000, pos.x};
		}
		else {
			return vector<int>{ scoreOfBoard(a_board, p2), pos.x};

		}
	}

	
	//int maxEval{};
	if (maximizing) {
		int maxEval = -100000;
		int path = -100;
		for (int i = 0; i < a_board.size(); i++) {
			if (isDropPointValid(a_board, i) == false) {
				continue;
			}

			int n = calcFallPos(a_board, i); // gets the fallposition
			char temp = a_board[i][n].item;
			a_board[i][n].item = p2; // the new gamestate

			int eval = minimax(a_board, Position(i, n), depth - 1, false)[0];
			if (maxEval < eval) {
				path = i;
			}
			maxEval = max(maxEval, eval); 

			a_board[i][n].item = temp; // reverses the change, for next iteration of for loop
		}
		return vector<int>{maxEval, path}/*maxEval*/;
	}
	else {
		int maxEval = 100000;
		int path = 100;
		for (int i = 0; i < a_board.size(); i++) {
			//vector<vector<Tile>> c_board = a_board;
			if (isDropPointValid(a_board, i) == false) {
				continue;
			}

			int n = calcFallPos(a_board, i); // gets the fallposition
			char temp = a_board[i][n].item;
			a_board[i][n].item = p1; // the new gamestate
			
			int eval = minimax(a_board, Position(i, n), depth - 1, true)[0];
			if (maxEval > eval) {
				path = i;
			}
			maxEval = min(maxEval, eval);

			a_board[i][n].item = temp;
		}
		return vector<int>{maxEval, path}/*maxEval*/;
	}
}

vector<Position> scoreOfTile(Position pos, vector<vector<Tile>> a_board) {
	//start with checking diagonal rays
	//checking //
	vector<Position> score{};


	castRay(pos, Position(1, 1), a_board, score);
	castRay(pos, Position(-1, -1), a_board, score);
	//cout << " // : " << score.size() << endl;
	score.push_back(pos);
	if (score.size() >= 4) return score;
		
	//checking \\ 
	score.clear();
	castRay(pos, Position(-1, +1), a_board, score);
	castRay(pos, Position(+1, -1), a_board, score);
	
	score.push_back(pos);
	if (score.size() >= 4) return score;

	//checking --
	score.clear();
	castRay(pos, Position(+1, 0), a_board, score);
	castRay(pos, Position(-1, 0), a_board, score);
	//cout << " -- : " << score.size() << endl;
	score.push_back(pos);
	if (score.size() >= 4) return score;

	//checking ||
	score.clear();
	castRay(pos, Position(0, +1), a_board, score);
	castRay(pos, Position(0, -1), a_board, score);
	//cout << " || : " << score.size() << endl;
	score.push_back(pos);
	if (score.size() >= 4) return score;


	return score;
}

int evalLine(vector<int> arr, char player) {
	int score{};
	for (int j = 0; j < arr.size() - 3; j++) { // loops thorough four wide segments at a time
		vector<int> arr2{};
		for (int h = 0; h < 4; h++) { // creates each segment
			arr2.push_back(arr[j + h]);
			score += evalRowOfFour(arr2, player);
		}
	}
	return score;

}

int evalRowOfFour(vector<int> row, char player) {
	int score{};

	int inRow{}, empy{}, opponent{};
	for (int i = 0; i < row.size(); i++) {
		if (row[i] == player) {
			inRow++;
		}
		else if (row[i] == EMPTY_PIECE) {
			empy++;
		}
		else {
			opponent++;
		}
	}


	if (inRow == 2 && empy == 2) {
		score += 2;
	}
	else if (inRow == 3 && empy == 1) {
		score += 5;
	}
	else if (opponent == 3 && empy == 1) {
		score += -4;
	}


	return score;

}

// GAME LOGIC ------------------------------

int Choice(vector<string> options, string title) {
	//defines current choice, this will be used as the return value and to determine where to draw the arrow
	int currentChoice{};
	bool accAns{};
	while (!accAns) {
		system("cls");
		cout << title << endl << endl;
		for (int i = 0; i < options.size(); i++) {// draws the options, and the arrow where the current choice is
			if (currentChoice == i) {
				cout << " ->   ";
			}
			else {
				cout << "    ";
			}
			cout << options[i] << endl;
		}

		char input = _getch();
		switch (tolower(input)) {
		case 'w': // if by pressing w, you go under 0, reset to top of array
			if (currentChoice - 1 < 0) {
				currentChoice = options.size() - 1;
				break;
			}
			currentChoice += -1;
			break;

		case 's':// if by pressing s, you go over options size, reset to bottom of array
			if (currentChoice + 1 >= options.size()) {
				currentChoice = 0;
				break;
			}
			currentChoice += 1;
			break;

		case ' ':
			accAns = true;
			break;

		default:
			break;
		}
	}
	return currentChoice;
}

void InitGame() {
	vector<vector<Tile>> board(7, vector<Tile>(6, Tile{ '*', false }));

	activePlayer = 'X';

	int AiAns = Choice({ "yes", "no" }, "Do You want to play agenst a AI? (AI is always second player)");
	bool activeAI{ false };
	if (AiAns == 0)
		activeAI = true;
	else
		activeAI = false;

	inputNames();
	mainGameloop(board, activeAI);
}

void inputNames() {
	cout << "Please input player one name : ";
	std::getline(cin, p1Name);
	cout << endl;

	cout << "Please input player two name : ";
	std::getline(cin, p2Name);
	cout << endl;
}

void mainGameloop(vector<vector<Tile>> a_board, bool a_activeAI) {
	bool finishedGame{ false };

	while (!finishedGame) {
		turn++;
		system("cls");
		int dropPoint{};
		if (activePlayer == p2 && a_activeAI) {
			vector<int>arr = minimax(a_board, Position(0, 0), 4, true);
			/*cout << "Score : " << arr[0] << "   Path : " << arr[1] << endl;
			system("pause");*/
			dropPoint = arr[1];
		}
		else {
			dropPoint = playerChooseSlot(a_board, a_activeAI);
			if (!isDropPointValid(a_board, dropPoint)) {
				cout << "Invalid placement, please choose again";
				addDotsToConsole(3, 500.f);
				continue;
			}
		}



		//int h = animateFall(a_board, dropPoint);
		int h = calcFallPos(a_board, dropPoint);
		animateFall(a_board, Position(dropPoint, h), 20);
		a_board[dropPoint][h].item = activePlayer;

		vector<Position> score = scoreOfTile(Position(dropPoint, h), a_board);
		cout << score.size() << endl;
		//system("pause");


		if (score.size() >= 4) {
			assignWinnerTilesBoard(a_board, score);

			string winningPlayer{};
			if (activePlayer == p1)
				winningPlayer = p1Name;
			else
				winningPlayer = p2Name;


			system("cls");
			drawBoard(a_board);
			cout << winningPlayer << " has won!" << endl;
			system("pause");
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

	//play again ? 
	int ch = Choice({ "Yes", "no" }, "Do you want to play again?");
	if (ch == 0) {
		InitGame();
	}
	else {

	}

}

int scoreOfBoard(vector<vector<Tile>> a_board, char player) { // for the score i used some of the scruture from this connect four alg https://github.com/KeithGalli/Connect4-Python/blob/503c0b4807001e7ea43a039cb234a4e55c4b226c/connect4_with_ai.py#L85
	//checking -- rows (x - axis)
	int score{};
	for (int i = 0; i < a_board[0].size(); i++) {
		//creating a vector for a row
		vector<int> arr{}; // creates an array for the entire row
		for (int j = 0; j < a_board.size(); j++) {
			arr.push_back(a_board[j][i].item);
		}
		score += evalLine(arr, player);
	}

	// checking the || collums (y - axis)
	for (int i = 0; i < a_board.size(); i++) {
		vector<int> arr{};
		for (int j = 0; j < a_board[0].size(); j++) {
			arr.push_back(a_board[i][j].item);
		}
		score += evalLine(arr, player);
	}

	return score;
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

void castRay(Position pos, Position dir, vector<vector<Tile>> a_board, vector<Position> &score) {
	char placedSym = a_board[pos.x][pos.y].item;
	//vector<position> score{};
	
	int i{ 1 };
	while (true) {
		if (isOutOfRange(Position(pos.x + dir.x * i, pos.y + dir.y * i), a_board))
			break;


		if (placedSym == a_board[pos.x + dir.x * i][pos.y + dir.y * i].item) { // shooting ray until it hits a opposite player or empty field
			score.push_back(Position(pos.x + dir.x * i, pos.y + dir.y * i));
		}
		else { // if it hits something other than the same symbol as the active player, break
			break;
		}
		i++;
	}
	return;
}

bool isOutOfRange(Position pos, vector<vector<Tile>> a_board) {
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


// VISUAL ----------------------------------
void addDotsToConsole(int dots, float duration) {
	for (int i = 0; i < dots; i++) {
		Sleep(duration);
		cout << '.';
	}
	Sleep(duration);
}

/// <summary>
/// animates the fall downwards for the newest piece
/// </summary>
/// <param name="a_board"></param>
/// <param name="a_dp"></param>
/// <returns></returns>
void animateFall(vector<vector<Tile>> a_board, Position pos, int stepDuration) {
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

int playerChooseSlot(vector<vector<Tile>> a_board, bool a_activeAI) {
	int currentChoice{ 0 };
	bool finished{ false };
	while (!finished) {
		system("cls");
		//displays the active player
		if (activePlayer == p1) {
			cout << p1Name;
		}
		else {
			cout << p2Name;
		}
		cout << "'s turn!";
		if (a_activeAI) {
			cout << "  (AI)";
		}
			
		cout << endl << endl;


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

void assignWinnerTilesBoard(vector<vector<Tile>> &a_board, vector<Position> poses) {
	for (int i = 0; i < poses.size(); i++) {
		a_board[ poses[i].x ][ poses[i].y ].winnerTile = true;
	}
}

