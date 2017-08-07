#include <regex>

#include "board.cpp"
#include "move.h"

class Game {
	Board board;
	std::vector<Move> moves;
	public:
		void init(std::string moves);
		void play();
};

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

	std::vector<std::string> singleMoves = split(moves, "(?!([0-9]+\\.))[a-zA-Z0-9+#=/\\-]+");
	for(uint i = 0; i < singleMoves.size(); i++) {
		std::cout << singleMoves[i] << std::endl;
		Move move(singleMoves[i], i % 2 == 0);
		this -> moves.push_back(move);
	}
	this -> play();
}

void Game::play() {
	int c = 0;
	int i = 0;
	while(1) {
		c = getchar();
		if(c == 10) {
			if(i == this -> moves.size() - 1)
				break;
			this -> board.apply(this -> moves[i]);
			this -> board.print();
			i ++;
		}else if(c == 27) {
			break;
		}
	}
}
