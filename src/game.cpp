#include <regex>

#include "board.cpp"
#include "move.cpp"

class Game {
	Board board;
	std::vector<Move> moves;
	public:
		void init(std::string moves);
};

std::vector<std::string> split(std::string s, std::string regexp) {
	std::regex e (regexp);
	std::regex_iterator<std::string::iterator> rit ( s.begin(), s.end(), e );
	std::regex_iterator<std::string::iterator> rend;

	std::vector<std::string> moves;
	while (rit!=rend) {
		moves.push_back(rit -> str());
		std::cout << rit -> str() << std::endl;
		++rit;
	}
	return moves;
}

void Game::init(std::string moves) {
	std::cout << "MOVES" << std::endl;
	std::cout << moves << std::endl;

	this -> board.init();

	std::vector<std::string> singleMoves = split(moves, "(?!([0-9]+\\.))[a-zA-Z0-9+#=-]+");
	for(uint i = 0; i < singleMoves.size(); i++) {
		std::cout << "I: " << i << ", ";
		Move move(singleMoves[i]);
		this -> moves.push_back(move);
	}
}
