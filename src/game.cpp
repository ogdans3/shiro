#include <regex>

#include "board.cpp"
#include "move.h"

class Game {
	Board board;
	std::vector<Move*> moves;
	int currentMove = 0;
	public:
		void init(std::string moves);
		void play();
		std::pair<int, int> nextMove();
		std::vector<Piece*> getState();
		void print();
		Move* getNextMove();
		bool dispose();
};

void Game::print() {
	this -> board.print();
}
Move* Game::getNextMove() {
	if(this -> currentMove >= this -> moves.size() - 1) {
		return NULL;
	}
	return this -> moves[this -> currentMove];
}
std::vector<Piece*> Game::getState() {
	return this -> board.getAllPieces();
}

std::vector<std::string> split(std::string s, std::string regexp) {
	std::regex e (regexp);
	std::regex_iterator<std::string::iterator> rit ( s.begin(), s.end(), e );
	std::regex_iterator<std::string::iterator> rend;

	std::vector<std::string> moves;
	while (rit!=rend) {
		moves.push_back(rit -> str());
		++rit;
	}
	return moves;
}

void Game::init(std::string moves) {
	this -> board.init();
	
	std::vector<std::string> singleMoves = split(moves, "(?!([0-9]+\\.))[a-zA-Z0-9+#=\\$/\\-]+");
	int count = 0;
	for(uint i = 0; i < singleMoves.size(); i++) {
		//This should ignore NAGs
		if(singleMoves[i].find("$") != std::string::npos) {
			continue;
		}
		Move* move = new Move(singleMoves[i], count % 2 == 0, count);
		this -> moves.push_back(move);
		count ++;
	}
}

std::pair<int, int> Game::nextMove() {
	std::pair<int, int> from = this -> board.apply(this -> moves[this -> currentMove]);
	this -> currentMove ++;
	return from;
}

void Game::play() {
	int c = 0;
	int i = 0;
	while(1) {
		c = getchar();
		if(c == 10) {
			if(i == this -> moves.size() - 1)
				break;
			this -> nextMove();
			this -> board.print();
			i ++;
		}else if(c == 27) {
			break;
		}
	}
}

bool Game::dispose() {
	for(uint i = 0; i < this -> moves.size(); i++) {
		delete this -> moves[i];
	}
	this -> board.dispose();
	return true;
}