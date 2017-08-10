#include <iostream>
#include <fstream>
#include <sstream>      // std::ostringstream

#include <sstream>
#include <string>
#include <algorithm>

#include "game.cpp"

int bracketCount = 0;
int parenthesisCount = 0;
int squareBracketCount = 0;
bool hasSetup(std::string s){
	if(s == "[SetUp \"1\"]")
		return true;
//	std::cout << s << ", " << "[SetUp \"1\"]" << (s == "[SetUp \"1\"]") << std::endl;
//	auto setup = s.find("[SetUp");
//	auto value = s.find("\"1\"") != std::string::npos;
//	std::cout << (setup != std::string::npos) << ", " << value << std::endl;
//	if(	setup != std::string::npos &&
//		value == 1)
//		return true;
	return false;
}

bool waitForEndOfLine = false;
std::string removeStuff(std::string s) {
	//std::cout << bracketCount << ", " << parenthesisCount << ", " << squareBracketCount << ", " << waitForEndOfLine << std::endl;

	std::ostringstream moves;
	for(int i = 0; i < s.size(); i++) {
		char c = s[i];
		if(c == '{') {
			if(squareBracketCount == 0)
				bracketCount ++;
			continue;
		}
		if(c == '(') {
			if(squareBracketCount == 0 && bracketCount == 0)
				parenthesisCount ++;
			continue;
		}
		if(c == '}') {
			if(bracketCount > 0)
				bracketCount --;
			continue;
		}
		if(c == ')') {
			if(parenthesisCount > 0 && bracketCount == 0)
				parenthesisCount --;
			continue;
		}
		if(c == '[' && i == 0) {
			if(squareBracketCount == 0 && bracketCount == 0 && parenthesisCount == 0)
				squareBracketCount ++;
			continue;
		}
		if(c == ']' && i == s.size() - 1) {
			if(squareBracketCount > 0)
				squareBracketCount --;
			continue;
		}
		if(c == ';') {
			if(squareBracketCount == 0 && bracketCount == 0 && parenthesisCount == 0)
				waitForEndOfLine = true;
			continue;
		}
		if(	bracketCount == 0 && parenthesisCount == 0 && squareBracketCount == 0 && waitForEndOfLine == false) {
			moves << c;
		}
	}
	return moves.str();
}

bool isEndOfGame(std::string str) {
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	if(str.size() > 0 && str.substr(str.size() - 1, str.size()) == "*")
		return true;
	if(str.size() < 3)
		return false;
	if(str.substr(str.size() - 3, str.size()) == "1-0")
		return true;
	if(str.substr(str.size() - 3, str.size()) == "0-1")
		return true;
	if(str.size() >= 7 && str.substr(str.size() - 7, str.size()) == "1/2-1/2")
		return true;
	return false;
}

std::string filePath = "";

Game* parseGame(std::string _filePath) {
	if(filePath != _filePath){
		filePath = _filePath;
		linesDone = 0;
	}
	bracketCount = 0;
	parenthesisCount = 0;
	squareBracketCount = 0;
	waitForEndOfLine = false;

	std::ifstream file(filePath);

	std::string line;
	std::vector<std::string> lines = std::vector<std::string>();
	std::string gameMoves = "";
	int i = 0;
	bool skipGame = false;
	while (std::getline(file, line)) {
		i ++;
		if(i <= linesDone)
			continue;
		linesDone ++;
	    std::istringstream iss(line);
	    //std::cout << "LB: " << line << std::endl;
	    skipGame = hasSetup(line) || skipGame;
	    line = removeStuff(line);
	    //std::cout << "L : " << line << std::endl;
	    //std::cout << lines.size() << std::endl;
	    lines.push_back(line);
		if(isEndOfGame(line)){
			std::ostringstream moves;
			for(uint i = 0; i < lines.size(); i++) {
				moves << lines[i] << " ";
			}
			lines.clear();
			if(moves.str() != " " && !skipGame) {
				gameMoves = moves.str();
				break;
			}
			skipGame = false;
		}
	}
	//std::cout << filePath << ", " << "Lines read: " << linesDone << std::endl;
	//std::cout << gameMoves << std::endl;
	file.close();
 	if(gameMoves == "")
 		return NULL;

 	Game* game = new Game();
	game -> init(gameMoves);
 	return game;
}