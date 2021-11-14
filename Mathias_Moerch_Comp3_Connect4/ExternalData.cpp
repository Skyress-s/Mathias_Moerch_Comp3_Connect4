#include "ExternalData.h"


vector<Player> loadFromLog(string filepath) {
	//checks if the file is empty
	if (true) {
		std::ifstream file{};
		file.open(filepath);

	}

	//creates a file vaiable and opens the file
	std::ifstream file{};
	file.open(filepath);

	if (file.peek() == std::ifstream::traits_type::eof()) { // this little snippet for checking if the file is empty is from https://kodlogs.com/blog/14054/c-check-if-file-is-empty
		return vector<Player>{};
	}

	//creates line
	vector<string> lines{};

	string line{};
	//reads lines, one line is the data of one player
	while (file.good()) {
		std::getline(file, line);
		lines.push_back(line);
	}

	//creates an empty vector of players (this is what we are going to return)
	vector<Player> players{};

	// loops though the lines
	for (int i = 0; i < lines.size(); i++) {
		// find the first ":" delimiter and saves the pos
		int pos1 = lines[i].find(':');
		string seg1 = lines[i].substr(0, pos1); // exstreacs string from start to the first ":" delimiter
		lines[i].erase(0, pos1 + 1); // adds one to also remove the ":"


		//same for the next two
		int pos2 = lines[i].find(':');
		string seg2 = lines[i].substr(0, pos2);
		lines[i].erase(0, pos2 + 1); // adds one to also remove the ":"


		int pos3 = lines[i].find(':');
		string seg3 = lines[i].substr(0, pos3);
		lines[i].erase(0, pos3 + 1); // adds one to also remove the ":"

		//creates an empty player
		Player temp("", 0, 0, 0, ' ');
		// assignt he name, wins, losses and color to the empty player
		temp.name = seg1;
		temp.wins = std::stoi(seg2);
		temp.losses = std::stoi(seg3);
		temp.color = std::stoi(lines[i]);

		//pushes it back tot he vector of players
		players.push_back(temp);

	}
	//returns the now populated players vector
	return players;
}

/// <summary>
/// func for overwriting the player data with the inputed players vector
/// </summary>
/// <param name="a_players"></param>
/// <param name="filepath"></param>
void writeToLog(vector<Player> a_players, string filepath) {

	//creates file vaiable and opens file
	std::ofstream file{};
	file.open(filepath, std::ios::out);

	//loops though all the players and writes them to file
	for (int i = 0; i < a_players.size(); i++) {
		string stringToWrite{};

		//creates the string to write (one line) example Joseph:41:14:1
		stringToWrite += a_players[i].name + ':';
		stringToWrite += std::to_string(a_players[i].wins) + ':';
		stringToWrite += std::to_string(a_players[i].losses) + ':';
		stringToWrite += std::to_string(a_players[i].color);

		//writes the line to the file
		file << stringToWrite;

		// add and enter on after all elements exept the last one
		//this is to prevent one empty line at the end, witch would cause errors for the loadFromFile func
		if (i != a_players.size() - 1) { 

			file << endl;
		}
	}
	return;
}

/// <summary>
/// adds or modifies a player if it alleready exists
/// </summary>
/// <param name="filepath"></param>
/// <param name="a_newP"></param>
void AddOrModifyPlayer(string filepath, Player a_newP) {
	//loads the players
	vector<Player> a_players = loadFromLog(filepath);


	//checks if the players exists
	for (int i = 0; i < a_players.size(); i++) {
		//if it does, add this players stats to the saved player and break the loop
		if (a_players[i].name == a_newP.name) {
			a_players[i].wins += a_newP.wins;
			a_players[i].losses += a_newP.losses;
			a_players[i].color = a_newP.color;
			break;
		}

		//if we are on the last element and havent found a matching player, add a ned one
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
