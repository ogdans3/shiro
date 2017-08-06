#include "board.cpp"

class Move{};

class Game {
	Board board;
	Move moves [];
	public:
		void init(std::string moves);
};

void Game::init(std::string moves) {
	this -> board.init();
}
