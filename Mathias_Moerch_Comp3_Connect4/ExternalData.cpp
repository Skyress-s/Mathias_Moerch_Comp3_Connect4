#include "ExternalData.h"


vector<Player> loadFromLog(string filepath) {
	// MULTIPLE OBJECT ATTEMPT
#pragma region MyRegion

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
