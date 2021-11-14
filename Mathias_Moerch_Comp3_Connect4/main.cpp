#include "DECLER.h"

Player playerOne( "Player ONE", 0, 0, 3, 'X' );
Player playerTwo( "PLAYER TWO", 0, 0, 5, 'O' ); 

int main() {
	mainMenu();
	return 0;
}

/// <summary>
/// function for clearing the cin buffer for errors and emptying it of left over data
/// Gotten from Johannes TA
/// </summary>
void ClearCin() { 
	std::cin.clear();    //Clears eventual errors from buffer
	std::cin.ignore(32767, '\n');    //clears the buffer if anything is there
}

// AI --------------------------------------

/// <summary>
/// Animates a board similar for the player choose drop slot
/// </summary>
/// <param name="a_board">the board you want to get the x size of</param>
/// <param name="dropPoint"> the target x pos to reach </param>
void AnimateAIDecition(std::vector<std::vector<Tile>>& a_board, int dropPoint, int& currentDP) {
	
	bool inPlace{ false };
	while (inPlace == false) { // while loop to animate until the global DP is equal to droppint argument

		system("cls");
		addColor(playerTwo.color);
		cout << "The AI is thinking..." << endl << endl << termcolor::reset; // flavor to display at the top of the screen

		//shows the fake dropslot bar [ ][ ][ ]...
		AIShowSlot(a_board, false, currentDP);
		//draws the board under to complete the game UI for AI turn
		drawBoard(a_board);

		//this isnt the most random number generator, but its easy and fast to implement srand() seed is in init();
		//adds this pause to give the AI more personality :)
		Sleep(rand() % 700);

		//add or minus to globalDP to match the inputtet droppoint gradualy
		if (currentDP > dropPoint) { 
			currentDP--;
		}
		else if (currentDP < dropPoint) {
			currentDP++;
		}
		else {
			inPlace = true; // when droppoint and DP matches, exit the while loop

		}

	}
}

/// <summary>
/// for the score i used some of the scruture from this connect four alg https://github.com/KeithGalli/Connect4-Python/blob/503c0b4807001e7ea43a039cb234a4e55c4b226c/connect4_with_ai.py#L85
/// refrence for alpha beta pruning https://www.youtube.com/watch?v=l-hh51ncgDI
/// Implementation of the minimax alg with pruning
/// the minimax assumes both players plays optimaly with the info they have, and minimax chooses the option that is best for the 
/// maximizing player (The AI in this case)
/// </summary>
/// <param name="a_board">the board state to evualiate</param>
/// <param name="pos"> the position of the last tile placed</param>
/// <param name="depth"></param>
/// <param name="maximizing"></param>
/// <returns>return vector<int>(2), 0 is score, 1 is path</returns>
vector<int> minimax(vector<vector<Tile>> a_board, Position pos, int depth, int alpha, int beta, bool maximizing) { // first is score, second is path
	//cals the score of the last tile placed
	int score = scoreOfTile(pos, a_board).size();
	
	//if its a termonal node / gamestate (one payer won or the depth is zero)
	if (depth == 0 || score >= 4) {
		// returns big minus value if the oppozing player wins
		// even tho the maximizing is true, we return a negative becouse minimax gets called further down to see the scores, so we need to comansate for the new call fo the minimax
		if (score >= 4 && maximizing == true) { 
			return  vector<int>{-1000, pos.x};
		}
		else if (score >= 4 && maximizing == false) {
			return  vector<int>{1000, pos.x};
		}
		else { // if no player won, return a interpretation of if the current board is good or bad for the AI
			//3 in a row with a open space in the AI favor, adds positive to the score
			return vector<int>{ scoreOfBoard(a_board, playerTwo.symbol), pos.x};
		}
	}

	if (maximizing) { // if maximizing players turn to place piece
		int maxEval = -100000; // very low max eval so the first eval is always bigger than this
		// path to minus one, could be 0 but will give errors if -100 path gets returned, will show to developer that 
		// something is wrong with the AI
		int path = -100; 
		for (int i = 0; i < a_board.size(); i++) {
			if (isDropPointValid(a_board, i) == false) { // checks if the AI can drop here, if not, skip over this iteration
				continue;
			}

			int n = calcFallPos(a_board, i); // gets the fallposition for this x (i) value
			char temp = a_board[i][n].item;
			a_board[i][n].item = playerTwo.symbol; // the new gamestate

			int eval = minimax(a_board, Position(i, n), depth - 1, alpha, beta, false)[0]; // new call of minimax with the modified gabe board
			a_board[i][n].item = temp; // reverses the change, for next iteration of for loop

			if (maxEval < eval) { // if eval is bigger than maxeval update the path of to match with the max eval 
				path = i;
			}
			maxEval = max(maxEval, eval); 

			//pruning. if the maximizing players sees that one player will go a 
			//route no matter what the other states might be, prune the rest of the loop (break)
			alpha = max(alpha, eval);
			if (beta <= alpha) {
				break;
			}
		}
		//return the best possible score with the coresponding path
		return vector<int>{maxEval, path};
	}
	else { // same to the if above to instead of - numbers and max, we use + numbers and min
		int maxEval = 100000;
		int path = 100;
		for (int i = 0; i < a_board.size(); i++) {
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
		return vector<int>{maxEval, path};
	}
}

/// <summary>
/// calculates the score of a tile placed (pos) in the gamevoard inputtet
/// this function could have been optimized alot, this is propaly why i cant use more iterations of the minimax AI 
/// </summary>
/// <param name="pos"></param>
/// <param name="a_board"></param>
/// <returns>vector of the eventual winning tiles, size() = score of tile</returns>
vector<Position> scoreOfTile(Position pos, vector<vector<Tile>> a_board) {
	//creates vector of positions to return
	vector<Position> score{};

	//casts "rays" in every direction that stops when they hit something that isnt a frendly tile or goes out of bounds

	//the up and left diagonal
	castRay(pos, Position(1, 1), a_board, score);
	castRay(pos, Position(-1, -1), a_board, score);
	score.push_back(pos);
	if (score.size() >= 4) return score; // if win return score
		
	//checking \\ 
	score.clear(); // clears the vector so it wont affect the next ray/line
	castRay(pos, Position(-1, +1), a_board, score);
	castRay(pos, Position(+1, -1), a_board, score);
	
	score.push_back(pos);
	if (score.size() >= 4) return score;

	//checking --
	score.clear();
	castRay(pos, Position(+1, 0), a_board, score);
	castRay(pos, Position(-1, 0), a_board, score);
	score.push_back(pos);
	if (score.size() >= 4) return score;

	//checking ||
	score.clear();
	castRay(pos, Position(0, +1), a_board, score);
	castRay(pos, Position(0, -1), a_board, score);
	score.push_back(pos);
	if (score.size() >= 4) return score;


	 // if none won return the last check as the score, therefore this functon should only be used to check if a player has won, 
	return score; // not how close a player is wo getting 4 in a row
}

/// <summary>
/// evualuates the scores of a line given to ut it, it evaluates the segments of four
/// </summary>
/// <param name="arr"></param>
/// <param name="player"></param>
/// <returns></returns>
int evalLine(vector<int> arr, char player) {
	int score{};
	//for loop to go though the line in segments of four and send the, to the eval Row of four
	for (int j = 0; j < arr.size() - 3; j++) { // loops thorough four wide segments at a time
		vector<int> arr2{};
		for (int h = 0; h < 4; h++) { // creates each segment
			arr2.push_back(arr[j + h]);
			score += evalRowOfFour(arr2, player);
		}
	}
	return score;
}

/// <summary>
/// gets feed vector with size of 4 and does the actual evaluation of if this is a good or bad
/// </summary>
/// <param name="row"></param>
/// <param name="player"></param>
/// <returns></returns>
int evalRowOfFour(vector<int> row, char player) {
	int score{};

	int inRow{}, empy{}, opponent{};
	for (int i = 0; i < row.size(); i++) { // ++ the apropiate int in acordance with what sort of tile it is
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

	//adds the score compared to how many of each peice or empty it is
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

/// <summary>
/// the operations to run before each game to init it
/// </summary>
/// <param name="activeAI"></param>
void InitGame(bool activeAI) {
	// creates the vector board with the magic numbers 6 height and 7 width, the rest of the game usese the size of this vector,
	// not more magic numbers
	vector<vector<Tile>> board(7, vector<Tile>(6, Tile{ EMPTY_PIECE, false })); 
	srand(time(0)); // sets the seed
	activePlayer = &playerOne; // sets the player
	//sets the name to AI if the player wants to play against a AI
	if (activeAI) { // setst player name to AI if ai is on
		playerTwo.name = "AI"; 
	}

	//runs the game
	mainGameloop(board, activeAI);
}

/// <summary>
/// the main game loop witch runs until the game is finished
/// </summary>
/// <param name="a_board"></param>
/// <param name="a_activeAI"></param>
void mainGameloop(vector<vector<Tile>> a_board, bool a_activeAI) {
	int turn{};
	int currentDP{3}; // current drop slot, for showing the current drop slot (x axis) position
	bool finishedGame{ false }; // set to true to finish a game
	while (!finishedGame) { // main game loop
		turn++;
		system("cls");
		int dropPoint{}; // local drop point, this is what the AI targets for incrementally moving to with currentDP

		if (activePlayer->symbol == playerTwo.symbol && a_activeAI) {
			//some flavor to show the AI is thinking
			system("cls");
			addColor(playerTwo.color);
			cout << "The AI is thinking..." << endl << endl;
			AIShowSlot(a_board, false, currentDP);
			drawBoard(a_board);

			//finds the optimal path for how deep into the future its looking
			vector<int>arr = minimax(a_board, Position(0, 0), 5, -10000, 10000, true);
			dropPoint = arr[1];

			//animates the movment of the slotdrop
			AnimateAIDecition(a_board, dropPoint, currentDP);
			
		}
		else { // player logic
			//player chooses a droppoint
			dropPoint = playerChooseSlot(a_board, a_activeAI, currentDP);
			//checks is the drop point is valid
			if (!isDropPointValid(a_board, currentDP)) { // if not, skip until next loop
				cout << "Invalid placement, please choose again";
				addDotsToConsole(3, 200.f); // flavor dots
				continue;
			}
		}
		
		
		int h = calcFallPos(a_board, currentDP);	//cals the fall height / position (y axis)
		animateFall(a_board, Position(currentDP, h), 20); // animates the fall
		a_board[currentDP][h].item = activePlayer->symbol; // changes the poition to the acording player symbol

		//calcs the score
		vector<Position> score = scoreOfTile(Position(currentDP, h), a_board);
		//if a player has won logic
		if (score.size() >= 4) {
			//assigns the boards bool tiles for winner tiles
			assignWinnerTilesBoard(a_board, score);

			//sets the appropiate win and losses scores for the players
			if (activePlayer->symbol == playerOne.symbol) {
				playerOne.wins = 1;
				playerTwo.losses = 1;
			}
			else {
				playerOne.losses = 1;
				playerTwo.wins = 1;
			}

			//draws the board agian but with the winning line highlighted
			system("cls");
			drawBoard(a_board);
			addColor(activePlayer->color);
			//displays the name of who won
			cout << "   " <<activePlayer->name << termcolor::reset << " was victorius !!!" << endl << endl;
			system("pause");
			finishedGame = true; // finished game 
		}

		//draw logic
		if (isBoardFull(a_board)) {
			system("cls");
			drawBoard(a_board); // draw board, display that its a draw
			cout << "   Board is " << termcolor::bright_blue << "FULL. Its a draw" << termcolor::reset << endl;
			system("pause");
			finishedGame = true;
		}
		
		toggleActivePlayer();//toggle active player at the end of each turn
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

/// <summary>
/// gives a heuristic value of the game board
/// </summary>
/// <param name="a_board"></param>
/// <param name="player"></param>
/// <returns></returns>
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

	//i dont check for diagonal scores becouse it takes more computing time, and the AI is alleredy quite slow

	return score;
}

/// <summary>
/// checks if inputted board is full or not
/// </summary>
/// <param name="a_board"></param>
/// <returns></returns>
bool isBoardFull(vector<vector<Tile>> a_board) {
	//checks the top row if all the pieces are full, if all are full, then all under also have to be filled
	int maxHeight = a_board[0].size() - 1;

	for (int i = 0; i < a_board.size(); i++) {
		if (a_board[i][maxHeight].item == EMPTY_PIECE) { // if ONE piece is empty, return false
			return false;
		}
	}

	return true;
}

/// <summary>
/// toggles what player is the active player
/// </summary>
void toggleActivePlayer() {
	//toggles back and forth between active players
	if (activePlayer->symbol == playerOne.symbol) {
		activePlayer = &playerTwo;
	}
	else {
		activePlayer = &playerOne;
	}
}

/// <summary>
/// calsculates the y position of where at new piece can be placed, based on a given x (dp) and a board
/// </summary>
/// <param name="a_board"></param>
/// <param name="a_dp"></param>
/// <returns></returns>
int calcFallPos(vector<vector<Tile>> a_board, int a_dp) {
	//sets the max height
	int currentHeight = a_board[0].size() - 1;
	while (true) {
		if (currentHeight <= 0) { // if we reach the bottom with of the loop, return height equal ground level
			return 0;
		}


		//if the uppermost position is taken, we would try too reack out of range, but we always check if we can drop here, 
		// so this is okay
		if (a_board[a_dp][currentHeight - 1].item != EMPTY_PIECE) { // if we hit not a empty, the tile above is the open one
			return currentHeight;
		}

		currentHeight--;
	}
}

/// <summary>
/// "Shoots" a ray in a specified direction and return the length of the amount of tiles traveled, 
/// and stops if it hits an enemy peice, air or edge
/// </summary>
/// <param name="pos"></param>
/// <param name="dir"></param>
/// <param name="a_board"></param>
/// <param name="score"></param>
void castRay(Position pos, Position dir, vector<vector<Tile>> a_board, vector<Position> &score) {
	//symbol at start of ray
	char placedSym = a_board[pos.x][pos.y].item;
	
	int i{ 1 };
	while (true) {
		//out of range?
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

/// <summary>
/// Checks if poition is out of range of board
/// </summary>
/// <param name="pos"></param>
/// <param name="a_board"></param>
/// <returns></returns>
bool isOutOfRange(Position pos, vector<vector<Tile>> a_board) {
	if (pos.x > a_board.size()-1 || pos.x < 0) { // is the x out of vector range
		return true;
	}
	else if (pos.y > a_board[0].size()-1 || pos.y < 0) { // is the y out of vector range
		return true;
	}
	else if (a_board[pos.x][pos.y].item == EMPTY_PIECE) { // did it hit an empty peice?
		return true;
	}
	return false; // else return false
}

/// <summary>
/// checks if a x coordinate of a board is a valid droppoint (atleast empty at the highest y pos in that row)
/// </summary>
/// <param name="a_board"></param>
/// <param name="a_dropPoint"></param>
/// <returns></returns>
bool isDropPointValid(vector<vector<Tile>> a_board, int a_dropPoint) {
	//getst the max height + 1
	int maxHeight = a_board[0].size();
	//gets the char of the upmost tile
	char potentialBlockedPos = a_board[a_dropPoint][maxHeight - 1].item;
	if (potentialBlockedPos == EMPTY_PIECE) { // if its empty
		return true;
	}
	else {
		return false;
	}
}


// VISUAL -----------------------

/// <summary>
/// adds some dots to the console with a delay
/// </summary>
/// <param name="dots"></param>
/// <param name="duration"></param>
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
	//gets the max height
	int currentHeight = a_board[0].size() - 1;

	//loops unitil we hit the proper height inputted
	while (currentHeight > pos.y) {
		a_board[pos.x][currentHeight].item = activePlayer->symbol;
		if (currentHeight < a_board[0].size() - 1) { // if we arent at the topmost tile, empty the one above it
			a_board[pos.x][currentHeight + 1].item = EMPTY_PIECE;
		}

		//draws the board with the fake dropsloat row
		system("cls");
		cout << endl << endl;
		int temp = 4;
		AIShowSlot(a_board, true, temp);
		drawBoard(a_board);
		Sleep(stepDuration); // puase for anim
		//minus one form current height, for next iteration
		currentHeight--;
	}

}

/// <summary>
/// func to draw the board that is inputted
/// </summary>
/// <param name="a_board"></param>
void drawBoard(vector<vector<Tile>> a_board) {
	cout << " ---------------------" << endl; // fluff
	// loop throgh the y axis
	for (int i = 0; i < a_board[0].size(); i++) {
		cout << "| ";
		//loop through all the x - axis 
		// draws from last element of the first layer first and the first element last
		//this is so the [x][y] of the board makes sense for a typical grapgh
		for (int j = 0; j < a_board.size(); j++) {
			Tile tile = a_board[j][a_board[0].size() - i - 1]; // gets the tile 
			//prints the color magenta if its a winner tile (somebody won)
			if (tile.winnerTile) {

				cout << termcolor::magenta;
			}
			//gives the players pieces their respective color
			else if (tile.item == playerOne.symbol) {
				addColor(playerOne.color);
			}
			else if (tile.item == playerTwo.symbol) {
				addColor(playerTwo.color);
			}

			//the actual item/character
			cout << a_board[j][a_board[0].size() - i - 1].item;
			cout << termcolor::reset;

			//spacing
			if (a_board.size() - 1 > j) {
				cout << "  ";
			}
		}
		cout << " |" << endl;
	}
	cout << " ---------------------" << endl;
}

/// <summary>
/// function for choosing a slot where to drop the next piece
/// </summary>
/// <param name="a_board"></param>
/// <param name="a_activeAI"></param>
/// <param name="currentDP"></param>
/// <returns></returns>
int playerChooseSlot(vector<vector<Tile>> a_board, bool a_activeAI, int& currentDP) {
	bool finished{ false };
	while (!finished) { // main loop for choosing
		system("cls");
		//displays the active player symbol 
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
			if (i == currentDP) {
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

		// switch case for the action the player took
		char ans = _getch();
		switch (tolower(ans)) {
		case 'd':
			if (currentDP + 1 < a_board.size()) { // if not out of range
				currentDP++;
			}
			else {
				currentDP = 0; // round robin to loop back
			}
			break;

		case 'a':
			if (currentDP - 1 >= 0) { // if not out of range, move down
				currentDP--;
			}
			else { // if out of range, round robid, loop back
				currentDP = a_board.size() - 1; 
			}
			break;

		case ' ': // confirm action
			finished = true;
			break;

		default:
			break;
		}
	}
	return currentDP;
}

/// <summary>
/// display a fake dropsclot bar, this is mainly for showing a bar when the ai is preforming actions
/// </summary>
/// <param name="a_board"></param>
/// <param name="empty"></param>
/// <param name="currentDP"></param>
void AIShowSlot(vector<vector<Tile>> a_board, bool empty, int& currentDP) {
	cout << " " << termcolor::reset; // spacing
	//loop for priting each [ ] induvudualy 
	for (size_t i = 0; i < a_board.size(); i++) { // draws the top bar
		if (i == currentDP && !empty) {
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

/// <summary>
/// assign the winner bool to active in the Tiles of the given board, based of the vector if winner positions gotten from score func
/// </summary>
/// <param name="a_board"></param>
/// <param name="poses"></param>
void assignWinnerTilesBoard(vector<vector<Tile>>& a_board, vector<Position> poses) {
	for (int i = 0; i < poses.size(); i++) {
		a_board[poses[i].x][poses[i].y].winnerTile = true;
	}
}

/// <summary>
/// adds color to the terminal based of the inputtet int color
/// in this program 0 = red, 1 = green and so on also applies for the colorChoice func
/// </summary>
/// <param name="col"></param>
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

/// <summary>
/// displays the stats of the players from the players.txt
/// </summary>
void stats() {
	system("cls");
	// adds the color
	//gets the players log in a vector
	vector<Player> players = loadFromLog(playersFile);

	//displays the stats title in bright_blue text
	cout << termcolor::bright_blue << "  Stats" << endl << endl;
	for (size_t i = 0; i < players.size(); i++) { // driplays the players stats and with colors
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

/// <summary>
/// options menu
/// </summary>
void options() {
	bool finished{ false };
	while (!finished) {
		string ans{};
		int act = Choice({ "Return to Main Menu",
			"Empty space symbol",
			"P1 : " + playerOne.name + " Color",
			"P2 : " + playerTwo.name + " Color",
			"P1 : " + playerOne.name + " Name",
			"P2 : " + playerTwo.name + " Name",
			"P1 : " + playerOne.name + " Symbol",
			"P2 : " + playerTwo.name + " Symbol"},
			"Options");
		//what to do basen on the inputted int
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

		case 6:
			inputPlayerSymbol(playerOne);
			break;

		case 7:
			inputPlayerSymbol(playerTwo);
			break;

		default:
			break;
		}
	}
}

/// <summary>
/// the main scrren
/// </summary>
void mainMenu() {
	bool finished{ false };
	while (!finished) {

		// choose and display choice
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


/// <summary>
/// Can choose between the options vector, 
/// the title will always print above the choices, space to accept answer where arrow is,
/// W and S to go up and down
/// </summary>
/// <param name="options"></param>
/// <param name="title"></param>
/// <returns> the index of the answer chosen </returns>
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
		cout << playerOne.name << "  - " << playerOne.symbol <<  endl << termcolor::reset;
		

		cout << "Player Two : ";
		addColor(playerTwo.color);
		cout << playerTwo.name << "  - " << playerTwo.symbol << endl << endl << termcolor::reset;


		for (int i = 0; i < options.size(); i++) {// draws the options, and the arrow where the current choice is
			if (currentChoice == i) { // if the arrow shoudl be here, current choice equals i
				cout << termcolor::bright_cyan;
				cout << " ->   ";
			}
			else { // if not, draw and slightly shorted than ---> empty space
				cout << "    ";

			}
			cout << options[i] << endl;
			cout << termcolor::reset;
		}

		//gets the users input
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

		case ' ': // if the user confirms their position
			accAns = true;
			break;

		default:
			break;
		}
	}
	return currentChoice;
}

/// <summary>
/// same func as Choice but specificly for choosing colors
/// i.e. the slots shows colors
/// </summary>
/// <param name="title"></param>
/// <returns></returns>
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
			if (currentChoice == i) { // draws arrow if i matches current choice
				addColor(options[i].second);
				cout << " ->   ";
			}
			else { // draws empty if i does not match current i
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

		case ' ': // when the user confirms their choice
			accAns = true;
			break;

		default:
			break;
		}
	}
	return currentChoice;
}

/// <summary>
/// input a name for the inputted player, by refrence to we dont have to return the modified player
/// </summary>
/// <param name="a_player"></param>
void inputName(Player& a_player) {
	system("cls");
	cout << "Please input P1 : ";
	addColor(a_player.color);
	cout << a_player.name;
	cout << termcolor::reset << " symbol : "; // flavor 
	addColor(a_player.color); // adds color so the name inputed by the player has the color of the player
	string name{};
	std::getline(cin, name); // input name
	cout << termcolor::reset; // resets color
	a_player.name = name; // sets the name for the player
	cout << endl; // an enter for spacing

}

/// <summary>
/// func for inputting a player char and assigning it to a specific player inputted
/// </summary>
/// <param name="a_player"></param>
void inputPlayerSymbol(Player& a_player) {

	cout << "Pleaase input a new symbol for "; // flavor
	addColor(a_player.color); //color for the player
	cout << a_player.name << termcolor::reset << " : ";
	addColor(a_player.color);
	//player inputs string/char
	string input{};
	cin >> input;
	a_player.symbol = input[0]; // takes only the first element of the string (in case the user inputted more than one char)
	cout << termcolor::reset;
	ClearCin();

}