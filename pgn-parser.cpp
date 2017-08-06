#include <iostream>
#include <fstream>
#include <sstream>      // std::ostringstream

#include <sstream>
#include <string>

#include "game.cpp"

bool isEmpty(std::string string) {
	return string.find_first_not_of("\t\n ") == std::string::npos;
}

std::vector<Game> parse(std::string filePath) {
	std::ifstream file(filePath);

	std::string line;
	std::vector<std::string> lines;
	std::vector<std::string> gameMoves;
	std::size_t found;
	while (std::getline(file, line)) {
	    std::istringstream iss(line);
	    found = line.find("[");
	    if(found != std::string::npos)
	    	continue;
	    found = line.find("{");
	    if(found != std::string::npos)
	    	continue;
	    lines.push_back(line);
		if(isEmpty(line)){
			std::ostringstream moves;
			for(uint i = 0; i < lines.size(); i++) {
				moves << lines[i] << " ";
			}
			lines.clear();
			if(moves.str() != " ")
				gameMoves.push_back(moves.str());
		}
	    if(gameMoves.size() > 0)
		    break;
	}
 	file.close();

 	std::vector<Game> games;
 	for(uint i = 0; i < gameMoves.size(); i++) {
 		Game game;
 		game.init(gameMoves[i]);
 		games.push_back(game);
 	}
 	return games;
}