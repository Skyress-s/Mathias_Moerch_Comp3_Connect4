#include "DECLER.h"

std::mutex mtx;



Player playerOne( "Player ONE", 0, 0, 3, 'X' );
Player playerTwo( "PLAYER TWO", 0, 0, 5, 'O' ); 


// MENUS --------------------------



int main() {

	/*inputName(playerOne);
	inputName(playerTwo);*/

	mainMenu();


	return 0;

}

void ClearCin() { 
	std::cin.clear();    //Clears eventual errors from buffer
	std::cin.ignore(32767, '\n');    //clears the buffer if anything is there
}

// AI --------------------------------------

void AnimateAIDecition(std::vector<std::vector<Tile>>& a_board, int dropPoint) {
	//animate where to place
	bool inPlace{ false };
	while (inPlace == false) {

		system("cls");
		addColor(playerTwo.color);
		cout << "The AI is thinking..." << endl << endl << termcolor::reset;

		AIShowSlot(a_board, false);
		drawBoard(a_board);

		//this isnt the most random number generator, but its easy and fast to implement srand() seed is in init();
		Sleep(rand() % 700);

		if (globalDP > dropPoint) {
			globalDP--;
		}
		else if (globalDP < dropPoint) {
			globalDP++;
		}
		else {
			inPlace = true;

		}

	}
}

/// <summary>
/// for the score i used some of the scruture from this connect four alg https://github.com/KeithGalli/Connect4-Python/blob/503c0b4807001e7ea43a039cb234a4e55c4b226c/connect4_with_ai.py#L85
/// Implementation of the minimax alg, choses based of the heuristic value of the board and end of game scores
/// </summary>
/// <param name="a_board"></param>
/// <param name="pos"></param>
/// <param name="depth"></param>
/// <param name="maximizing"></param>
/// <returns>return vector<int>(2), 0 is score, 1 is path</returns>
vector<int> minimax(vector<vector<Tile>> a_board, Position pos, int depth, int alpha, int beta, bool maximizing) { // first is score, second is path

	int score = scoreOfTile(pos, a_board).size();
	//int score = 1;
	if (depth == 0 || score >= 4) {
		if (score >= 4 && maximizing == true) {
			/*drawBoard(a_board);
			cout << "LOSS!" << endl;
			system("pause");*/
			return  vector<int>{-1000, pos.x};
		}
		else if (score >= 4 && maximizing == false) {
			/*drawBoard(a_board);
			cout << "AI WON!" << endl;
			system("pause");*/
			return  vector<int>{1000, pos.x};
		}
		else {
			return vector<int>{ scoreOfBoard(a_board, playerTwo.symbol), pos.x};
			//return {0, pos.x};
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
			a_board[i][n].item = playerTwo.symbol; // the new gamestate

			int eval = minimax(a_board, Position(i, n), depth - 1, alpha, beta, false)[0];
			a_board[i][n].item = temp; // reverses the change, for next iteration of for loop

			if (maxEval < eval) {
				path = i;
			}
			//eval
			maxEval = max(maxEval, eval); 

			//pruning
			alpha = max(alpha, eval);
			if (beta <= alpha) {
				break;
			}


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
			a_board[i][n].item = playerOne.symbol; // the new gamestate
			
			int eval = minimax(a_board, Position(i, n), depth - 1, alpha, beta, true)[0];
			a_board[i][n].item = temp;

			if (maxEval > eval) {
				path = i;
			}
			//eval
			maxEval = min(maxEval, eval);


			//pruning
			beta = min(beta, eval);
			if (beta <= alpha) {
				break;
			}


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



void InitGame(bool activeAI) {
	vector<vector<Tile>> board(7, vector<Tile>(6, Tile{ EMPTY_PIECE, false }));
	srand(time(0));
	activePlayer = &playerOne;
	if (activeAI) {
		playerTwo.name = "AI";
	}

	mainGameloop(board, activeAI);
}

void inputName(Player& a_player) {
	system("cls");
	cout << "Please input player's one name : ";
	addColor(a_player.color);
	string name{};
	std::getline(cin, name);
	cout << termcolor::reset;
	a_player.name = name;


	cout << endl;
	/*system("cls");
	if (!_activeAI) {
		cout << "Please input player two name : ";
		addColor(playerTwo.color);
		std::getline(cin, name);
		playerTwo.name = name;
		cout << endl << termcolor::reset;
	}
	else {
		playerTwo.name = "AI";
	}*/
}

void mainGameloop(vector<vector<Tile>> a_board, bool a_activeAI) {
	int add = 1;
	bool finishedGame{ false };
	while (!finishedGame) {
		turn++;
		system("cls");
		int dropPoint{};
		if (activePlayer->symbol == playerTwo.symbol && a_activeAI) {
			//some flavor to show the AI is thinking
			system("cls");
			addColor(playerTwo.color);
			cout << "The AI is thinking..." << endl << endl;
			AIShowSlot(a_board, false);
			drawBoard(a_board);

			vector<int>arr = minimax(a_board, Position(0, 0), 5, -10000, 10000, true);
			dropPoint = arr[1];

			
			AnimateAIDecition(a_board, dropPoint);


#pragma region MyRegion



			//vector<vector<vector<Tile>>> threadBoards{}; // creates different board for each possible move and then adds them to a thread
			//for (int i = 0; i < a_board.size(); i++) {
			//	if (isDropPointValid(a_board, i) == false) // if it isnt a valid droppoint, do not make a instance for it
			//		continue;
			//	threadBoards.push_back(a_board);

			//	int y = calcFallPos(a_board, i);
			//	threadBoards[i][i][y].item = p2;

			//}
			//double start{ omp_get_wtime() };
			//vector<std::thread> threads{};
			//for (int i = 0; i < threadBoards.size(); i++) {
			//	threads.push_back(std::thread(minimax, threadBoards[i], Position(0, 0), 3, false));
			//}

			//for (int i = 0; i < threads.size(); i++) {
			//	threads[i].join();
			//}
			//cout << " Time: " << omp_get_wtime() - start << endl;

			//cout << "finsihed!" << endl;
			//system("pause");


			//new method
			/*vector<vector<int>> paths{};

			std::thread p1(minimax, a_board, Position(0, 0), 4, false);
			std::thread p1(minimax, a_board, Position(0, 0), 4, false);
			std::thread p1(minimax, a_board, Position(0, 0), 4, false);
			std::thread p1(minimax, a_board, Position(0, 0), 4, false);
			std::thread p1(minimax, a_board, Position(0, 0), 4, false);
			std::thread p1(minimax, a_board, Position(0, 0), 4, false);
			std::thread p1(minimax, a_board, Position(0, 0), 4, false);*/

#pragma endregion

		}
		else {
			dropPoint = playerChooseSlot(a_board, a_activeAI);
			if (!isDropPointValid(a_board, dropPoint)) {
				cout << "Invalid placement, please choose again";
				addDotsToConsole(3, 200.f);
				continue;
			}
		}



		//int h = animateFall(a_board, dropPoint);
		int h = calcFallPos(a_board, dropPoint);
		animateFall(a_board, Position(dropPoint, h), 20);
		a_board[dropPoint][h].item = activePlayer->symbol;

		vector<Position> score = scoreOfTile(Position(dropPoint, h), a_board);
		cout << score.size() << endl;
		//system("pause");

		//if a player has won logic
		if (score.size() >= 4) {
			assignWinnerTilesBoard(a_board, score);

			if (activePlayer->symbol == playerOne.symbol) {
				playerOne.wins = 1;
				playerTwo.losses = 1;
			}
			else {
				playerOne.losses = 1;
				playerTwo.wins = 1;
			}



			system("cls");
			drawBoard(a_board);
			addColor(activePlayer->color);
			cout << "   " <<activePlayer->name << termcolor::reset << " was victorius !!!" << endl << endl;
			system("pause");
			finishedGame = true;
		}

		//draw logic
		if (isBoardFull(a_board)) {
			system("cls");
			drawBoard(a_board);
			cout << "   Board is " << termcolor::bright_blue << "FULL. Its a draw" << termcolor::reset << endl;
			system("pause");
			finishedGame = true;
		}


		scoreOfBoard(a_board, activePlayer->symbol);
		toggleActivePlayer();
	}


	//update the players.txt file
	
	

	AddOrModifyPlayer(playersFile, playerOne);
	AddOrModifyPlayer(playersFile, playerTwo);



	//play again ? 
	int ch = Choice({ "Yes", "no" }, "Return to Main Menu?");
	if (ch == 1) {
		exit(0);
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
			if (a_board[i][j].item == EMPTY_PIECE) {
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
	if (activePlayer->symbol == playerOne.symbol) {
		activePlayer = &playerTwo;
	}
	else {
		activePlayer = &playerOne;
	}
}

int calcFallPos(vector<vector<Tile>> a_board, int a_dp) {

	int currentHeight = a_board[0].size() - 1;
	while (true) {
		if (currentHeight <= 0) {
			return 0;
		}

		if (a_board[a_dp][currentHeight - 1].item != EMPTY_PIECE) {
			return currentHeight;
		}

		currentHeight--;
	}
}

void castRay(Position pos, Position dir, vector<vector<Tile>> a_board, vector<Position> &score) {
	char placedSym = a_board[pos.x][pos.y].item;
	
	
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
	else if (a_board[pos.x][pos.y].item == EMPTY_PIECE) {
		return true;
	}
	return false;
}

bool isDropPointValid(vector<vector<Tile>> a_board, int a_dropPoint) {
	int maxHeight = a_board[0].size();
	char potentialBlockedPos = a_board[a_dropPoint][maxHeight - 1].item;
	if (potentialBlockedPos == EMPTY_PIECE) {
		return true;
		system("pause");
	}
	else {
		return false;
	}
}


// VISUAL -----------------------

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
		a_board[pos.x][currentHeight].item = activePlayer->symbol;
		if (currentHeight < a_board[0].size() - 1) {
			a_board[pos.x][currentHeight + 1].item = EMPTY_PIECE;
		}

		system("cls");
		cout << endl << endl;
		AIShowSlot(a_board, true);
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
			else if (tile.item == playerOne.symbol) {
				addColor(playerOne.color);
			}
			else if (tile.item == playerTwo.symbol) {
				addColor(playerTwo.color);
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
		if (activePlayer->symbol == playerOne.symbol) {
			addColor(playerOne.color);
			cout << playerOne.name;
		}
		else {
			addColor(playerTwo.color);
			cout << playerTwo.name;
		}
		cout << "'s turn!" << termcolor::reset;
		if (a_activeAI && activePlayer->symbol == playerTwo.symbol) {
			cout << "  (AI)";
		}

		cout << endl << endl;


		//draws the choice bar
		cout << " ";
		for (int i = 0; i < a_board.size(); i++) {
			char content{ ' ' };
			if (i == globalDP) {
				content = activePlayer->symbol;
				if (content == playerOne.symbol) {
					addColor(playerOne.color);
				}
				else if (content == playerTwo.symbol) {
					addColor(playerTwo.color);
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
			if (globalDP + 1 < a_board.size()) {
				globalDP++;
			}
			else {
				globalDP = 0;
			}
			break;

		case 'a':
			if (globalDP - 1 >= 0) {
				globalDP--;
			}
			else {
				globalDP = a_board.size() - 1;
			}
			break;

		case ' ':
			finished = true;
			break;

		default:
			break;
		}
	}
	return globalDP;
}

void AIShowSlot(vector<vector<Tile>> a_board, bool empty) {
	cout << " " << termcolor::reset; // spacing
	for (size_t i = 0; i < a_board.size(); i++) { // draws the top bar
		if (i == globalDP && !empty) {
			addColor(playerTwo.color);
			cout << '[' << playerTwo.symbol;
		}
		else {
			cout << '[' << ' ';
		}
		cout << ']' << termcolor::reset;
	}
	cout << endl;
}

void assignWinnerTilesBoard(vector<vector<Tile>>& a_board, vector<Position> poses) {
	for (int i = 0; i < poses.size(); i++) {
		a_board[poses[i].x][poses[i].y].winnerTile = true;
	}
}

void addColor(int col) {
	switch (col) {
	case 0:
		cout << termcolor::red;
		break;

	case 1:
		cout << termcolor::green;
		break;

	case 2:
		cout << termcolor::blue;
		break;

	case 3:
		cout << termcolor::bright_red;
		break;

	case 4:
		cout << termcolor::bright_green;
		break;
	case 5:
		cout << termcolor::bright_blue;
		break;

	default:
		break;
	}
}

void stats() {
	system("cls");
	// adds the color


	vector<Player> players = loadFromLog(playersFile);

	cout << termcolor::bright_blue << "  Stats" << endl << endl;
	for (size_t i = 0; i < players.size(); i++) {
		addColor(players[i].color);
		cout << "    Player : " << players[i].name << endl;
		cout << "    Wins   : " << players[i].wins << endl;
		cout << "    losses : " << players[i].losses << endl;
		cout << endl << termcolor::reset;
	}

	addColor(4);
	cout << " ->  Return to Meny" << endl << endl << termcolor::reset;

	system("pause");

}

void options() {
	bool finished{ false };
	while (!finished) {
		string ans{};
		int act = Choice({ "Return to Main Menu",
			"Empty space symbol",
			"P1 : " + playerOne.name + " Color",
			"P2 : " + playerTwo.name + " Color",
			"P1 : " + playerOne.name + " Name",
			"P2 : " + playerTwo.name + " Name" }, "Options");
		switch (act) {
		case 0:
			finished = true;
			break;

		case 1:
			system("cls");
			cout << "Please enter a new Empty Space Symbol : ";
			cin >> ans;
			ClearCin();
			EMPTY_PIECE = ans[0];
			break;

		case 2:
			playerOne.color = colorChoice({ playerOne.name, playerOne.color });

			break;

		case 3:
			playerTwo.color = colorChoice({ playerTwo.name, playerTwo.color });
			break;

		case 4:
			inputName(playerOne);
			break;

		case 5:
			inputName(playerTwo);
			break;

		default:
			break;
		}
	}
}

void mainMenu() {
	bool finished{ false };
	while (!finished) {

		//vector<std::pair<string, int>> choices = { {"Start", 0}, {"Options", 0}, {"Stats", 0}, {"Exit", 0} };
		int act = Choice({ "Play Duos", "Player Against AI", "Options", "Stats", "Exit" }, "Welcome to Connect 4");
		switch (act) {
		case 0:
			InitGame(false);
			break;

		case 1:
			InitGame(true);
			break;

		case 2:
			options();
			break;

		case 3:
			stats();
			break;

		case 4:
			finished = true;
			break;

		default:
			break;
		}
	}

}

int Choice(vector<string> options, string title) {
	//defines current choice, this will be used as the return value and to determine where to draw the arrow
	int currentChoice{};
	bool accAns{};
	while (!accAns) {
		system("cls");
		cout << title << endl << endl;
		//displays the players and their colors
		cout << "Player One : ";
		addColor(playerOne.color);
		cout << playerOne.name << endl << termcolor::reset;
		cout << "Player Two : ";
		addColor(playerTwo.color);
		cout << playerTwo.name << endl << endl << termcolor::reset;


		for (int i = 0; i < options.size(); i++) {// draws the options, and the arrow where the current choice is
			if (currentChoice == i) {
				cout << termcolor::bright_cyan;
				cout << " ->   ";
			}
			else {
				cout << "    ";

				////if an optional color was inputed
				//if (colors.size()>0) {
				//	if (colors[i] =! -1) {
				//		addColor(colors[i]);
				//	}
				//}


			}
			cout << options[i] << endl;
			cout << termcolor::reset;
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

int colorChoice(std::pair<string, int> title) {
	vector<std::pair<string, int>> options{
		{"Red", 0},
		{"Green", 1},
		{"Blue", 2},
		{"Bright Red", 3},
		{"Bright Green", 4},
		{"Bright Blue", 5}
	};

	//defines current choice, this will be used as the return value and to determine where to draw the arrow
	int currentChoice{};
	bool accAns{};
	while (!accAns) {
		system("cls");

		//title color
		addColor(currentChoice);
		cout << "   " << title.first << endl << endl << termcolor::reset;
		for (int i = 0; i < options.size(); i++) {// draws the options, and the arrow where the current choice is
			if (currentChoice == i) {
				addColor(options[i].second);
				cout << " ->   ";
			}
			else {
				cout << "    ";
			}
			cout << options[i].first << endl;
			cout << termcolor::reset;
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



