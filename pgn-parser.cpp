#include <iostream>
#include <fstream>

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
	std::size_t found;
	while (std::getline(file, line)) {
	    std::istringstream iss(line);
	    found = line.find("[");
	    if(found != std::string::npos)
	    	continue;
	    found = line.find("{");
	    if(found != std::string::npos)
	    	continue;
		if(isEmpty(line))
			continue;
	    lines.push_back(line);
	}
 	file.close();
 	std::cout << lines.size() << std::endl;

 	std::vector<Game> games;
 	for(int i = 0; i < lines.size(); i++) {
 		Game game;
 		games.push_back(game);
 	}
 	return games;
}