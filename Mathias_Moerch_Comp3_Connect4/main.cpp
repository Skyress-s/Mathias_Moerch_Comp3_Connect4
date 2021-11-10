#include "DECLER.h"
#include <sstream>
#include <typeindex>
#include <mutex>
#include <omp.h>
#include "Player.h"
// EXTERNAL DATA -----------------------------
std::mutex mtx;
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


Player playerOne( "PlayerONE", 0, 0, 3, 'X' );
Player playerTwo( "PLAYER TWO", 0, 0, 5, 'O' ); 

vector<Player> loadFromLog(string filepath) {
	// MULTIPLE OBJECT ATTEMPT
#pragma region MyRegion



	

	////read!
	//vector<Player> players{};

	//std::ifstream file2{};
	//file2.open(filepath);

	////while (file2.good()) {
	//	Player temp("empty", 0, 0);

	//	file2.read((char*)&temp, sizeof(p1));
	//	players.push_back(temp);

	////}

	//
	//file2.close();

	//for (int i = 0; i < players.size(); i++) {
	//	cout << "name : " << players[i].name << " wins : " << players[i].wins << " losses : " << players[i].losses << endl;
	//}


	//return vector<Player*>{};


	//single object read write ---------------

	//write -------------------------------

	//vector<Player*> players{};
	//
	//Player p1("Konas", 31230, 12114);


	//std::ofstream file(filepath);
	////std::ofstream file{};
	////file.open(filepath, std::ios::app);

	//file.write((char*)&p1, sizeof(p1));

	//file.close();

	//read --------------


	//Player* p2 = new Player("", 0, 0);

	////std::ifstream file2(filepath);
	//std::ifstream file2;
	//file2.open(filepath, std::ios::in);


	//file2.read((char*)& p2, sizeof(p2));
	////file2 >> p2;


	//file2.close();
	////cout << "name : " << p2.name << " wins : " << p2.wins << " losses : " << p2.losses << endl;
	//cout << "name : " << p2->name << " wins : " << p2->wins << " losses : " << p2->losses << endl;

#pragma endregion
	//checks if the file is empty
	if (true) {
		std::ifstream file{};
		file.open(filepath);
		
	}

	std::ifstream file{};
	file.open(filepath);

	if (file.peek() == std::ifstream::traits_type::eof()) { // this little snippet for checking if the file is empty is from https://kodlogs.com/blog/14054/c-check-if-file-is-empty
		return vector<Player>{};
	}


	vector<string> lines{};

	string line{};
	while (file.good()) {
		std::getline(file, line);
		lines.push_back(line);
	}

	vector<Player> players{};

	for (int i = 0; i < lines.size(); i++) {


		int pos1 = lines[i].find(':');
		string seg1 = lines[i].substr(0, pos1);
		lines[i].erase(0, pos1 + 1); // adds one to also remove the ":"


		int pos2 = lines[i].find(':');
		string seg2 = lines[i].substr(0, pos2);
		lines[i].erase(0, pos2 + 1); // adds one to also remove the ":"


		int pos3 = lines[i].find(':');
		string seg3 = lines[i].substr(0, pos3);
		lines[i].erase(0, pos3 + 1); // adds one to also remove the ":"


		Player temp("", 0, 0, 0, ' ');
		temp.name = seg1;
		temp.wins = std::stoi(seg2);
		temp.losses = std::stoi(seg3);
		temp.color = std::stoi(lines[i]);

		players.push_back(temp);

	}
	return players;
}


void writeToLog(vector<Player> a_players, string filepath) {
#pragma region maisam Serializing classes



	////write
	//vector<Player*> players{};
	//players.push_back(new Player("test", 100, 0));
	//players.push_back(new Player("Mei", 10, 123));
	//players.push_back(new Player("Sam", 0, 0123));
	//players.push_back(new Player("Thomas", 1130, 51));

	//std::ofstream file{};
	//file.open(filepath);

	//for (int i = 0; i < players.size(); i++) {
	//	file.write((char*)&players[i], sizeof(players[i])); // the size of needed to be updated

	//}

	////deletes all the excess object
	//for (int i = 0; i < players.size(); i++) {
	//	delete(players[i]);
	//}

	//file.close();
#pragma endregion

	

	std::ofstream file{};
	file.open(filepath, std::ios::out);

	for (int i = 0; i < a_players.size(); i++) {
		string stringToWrite{};
		
		stringToWrite += a_players[i].name + ':';
		stringToWrite += std::to_string(a_players[i].wins) + ':';
		stringToWrite += std::to_string(a_players[i].losses) + ':';
		stringToWrite += std::to_string(a_players[i].color);

		file << stringToWrite;
		if (i != a_players.size() - 1) {

			file << endl;
		}
	}

	return;
}

void stats() {
	system("cls");
	// adds the color
	addColor(4);
	cout << " ->  Return to Meny" << endl << endl << termcolor::reset;

	vector<Player> players = loadFromLog(playersFile);

	for (size_t i = 0; i < players.size(); i++) {
		addColor(players[i].color);
		cout << "    Player : " << players[i].name << endl;
		cout << "    Wins   : " << players[i].wins << endl;
		cout << "    losses : " << players[i].losses << endl;
		cout << endl << termcolor::reset;
	}

	system("pause");

}

void AddOrModifyPlayer(string filepath, Player a_newP) {
	//loads the players
	vector<Player> a_players = loadFromLog(filepath);

	

	for (int i = 0; i < a_players.size(); i++) {
		if (a_players[i].name == a_newP.name) {
			a_players[i].wins += a_newP.wins;
			a_players[i].losses += a_newP.losses;
			a_players[i].color = a_newP.color;
			break;
		}

		if (i == a_players.size() - 1) {
			a_players.push_back(a_newP);
			break;
		}
	}

	//edge case if the file is empty
	if (a_players.size() == 0) {
		a_players.push_back(a_newP);
	}


	//saves them agian
	writeToLog(a_players, filepath);
}

void setPlayerColor() {

}

void options() {
	bool finished{ false };
	while (!finished) {
		string ans{};
		int act = Choice({"Return to Main Menu", "Empty space symbol", "Player One Color", "Player Two Color"}, "Options");
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
			playerOne.color = colorChoice({ playerOne.name, 0});

			break;

		case 3:
			playerOne.color = colorChoice({ playerTwo.name, 0 });
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
		int act = Choice({"Start", "Options", "Stats", "Exit"}, "Welcome to Connect 4 !!!");
		switch (act) {
		case 0:
			InitGame();
			break;

		case 1:
			options();
			break;

		case 2:
			stats();
			break;

		case 3:
			finished = true;
			break;

		default:
			break;
		}
	}

}

int main() {

	/*vector<Player> players{};
	players.push_back(Player("test", 100, 0));
	players.push_back(Player("Mei", 10, 123));
	players.push_back(Player("Sam", 0, 0123));
	players.push_back(Player("Thomas", 1130, 51));
	WriteToLog(players, playersFile);

	vector<Player> players2 = LoadFromLog("Players.txt");
	for (int i = 0; i < players2.size(); i++) {
		players2[i].printInfo();
	}*/

	//LoadFromLog("Players.txt");

	//for (int i = 0; i < players.size(); i++) {
	//	cout << "name : " << players[i]->name << " wins : " << players[i]->wins << " losses : " << players[i]->losses << endl;
	//}
	

	//return 0;

	mainMenu();


	return 0;

}

void ClearCin() { 
	std::cin.clear();    //Clears eventual errors from buffer
	std::cin.ignore(32767, '\n');    //clears the buffer if anything is there
}

// AI --------------------------------------

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

int Choice(vector<string> options, string title) {
	//defines current choice, this will be used as the return value and to determine where to draw the arrow
	int currentChoice{};
	bool accAns{};
	while (!accAns) {
		system("cls");
		cout << title << endl << endl;
		for (int i = 0; i < options.size(); i++) {// draws the options, and the arrow where the current choice is
			if (currentChoice == i) {
				cout << termcolor::bright_cyan;
				cout << " ->   ";
			}
			else {
				cout << "    ";
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

int colorChoice( std::pair<string, int> title) {
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

void InitGame() {
	vector<vector<Tile>> board(7, vector<Tile>(6, Tile{ EMPTY_PIECE, false }));
	srand(time(0));

	int AiAns = Choice({ "yes", "no" }, "Do You want to play agenst a AI? (AI is always second player)");
	bool activeAI{ false };
	if (AiAns == 0)
		activeAI = true;
	else
		activeAI = false;

	inputName(activeAI);
	mainGameloop(board, activeAI);
}

void inputName(bool _activeAI) {
	system("cls");
	cout << "Please input player one name : ";
	addColor(playerOne.color);
	string name{};
	std::getline(cin, name);
	cout << termcolor::reset;
	playerOne.name = name;


	cout << endl;
	system("cls");
	if (!_activeAI) {
		cout << "Please input player two name : ";
		addColor(playerTwo.color);
		std::getline(cin, name);
		playerTwo.name = name;
		cout << endl << termcolor::reset;
	}
	else {
		playerTwo.name = "AI";
	}
}

void mainGameloop(vector<vector<Tile>> a_board, bool a_activeAI) {
	bool finishedGame{ false };
	activePlayer = playerOne.symbol;
	while (!finishedGame) {
		turn++;
		system("cls");
		int dropPoint{};
		if (activePlayer == playerTwo.symbol && a_activeAI) {
			//some flavor to show the AI is thinking
			system("cls");
			addColor(playerTwo.color);
			cout << "The AI is thinking..." << endl << endl;
			AIShowSlot(a_board, false);
			drawBoard(a_board);

			vector<int>arr = minimax(a_board, Position(0, 0), 5, -10000, 10000, true);
			dropPoint = arr[1];

			//animate where to place
			bool inPlace{ false };
			
			while (inPlace == false) {

				system("cls");
				addColor(playerTwo.color);
				cout << playerTwo.name << "'s turn!!!" << endl << endl << termcolor::reset;

				AIShowSlot(a_board, false);
				drawBoard(a_board);

				Sleep(rand() % 1700);

				if (globalDP > dropPoint) {
					globalDP--;
				}
				else if (globalDP < dropPoint) {
					globalDP++;
				}
				else {
					inPlace = true;
					
				}
				//this isnt the most random number generator, but its easy and fast to implement srand() seed is in init();

			}


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
		a_board[dropPoint][h].item = activePlayer;

		vector<Position> score = scoreOfTile(Position(dropPoint, h), a_board);
		cout << score.size() << endl;
		//system("pause");


		if (score.size() >= 4) {
			assignWinnerTilesBoard(a_board, score);

			string winningPlayer{};
			if (activePlayer == playerOne.symbol)
				winningPlayer = playerOne.name;
			else
				winningPlayer = playerTwo.name;


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


	//update the players.txt file
	int add = 1;
	if (activePlayer == playerTwo.symbol) {
		playerOne.wins = 1;
		playerTwo.losses = 1;
	}
	else {
		playerOne.losses = 1;
		playerTwo.wins = 1;
	}

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
	if (activePlayer == playerOne.symbol) {
		activePlayer = playerTwo.symbol;
	}
	else {
		activePlayer = playerOne.symbol;
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
		if (activePlayer == playerOne.symbol) {
			addColor(playerOne.color);
			cout << playerOne.name;
		}
		else {
			addColor(playerTwo.color);
			cout << playerTwo.name;
		}
		cout << "'s turn!" << termcolor::reset;
		if (a_activeAI && activePlayer == playerTwo.symbol) {
			cout << "  (AI)";
		}
			
		cout << endl << endl;


		//draws the choice bar
		cout << " ";
		for (int i = 0; i < a_board.size(); i++) {
			char content{ ' ' };
			if (i == globalDP) {
				content = activePlayer;
				if (activePlayer == playerOne.symbol) {
					addColor(playerOne.color);
				}
				else if (activePlayer == playerTwo.symbol) {
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
			if (globalDP -1 >= 0) {
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
	cout << " "; // spacing
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

void assignWinnerTilesBoard(vector<vector<Tile>> &a_board, vector<Position> poses) {
	for (int i = 0; i < poses.size(); i++) {
		a_board[ poses[i].x ][ poses[i].y ].winnerTile = true;
	}
}


