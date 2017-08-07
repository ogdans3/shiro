#include "player.cpp"
#include <sstream>      // std::ostringstream

#include "move.h"

class Board {
	Player white;
	Player black;
	public:
		void init();
		void print();
		void apply(Move);
};

void Board::init() {
	this -> white.init(true);
	this -> black.init(false);
}

void Board::apply(Move move) {
	if(move.whiteMove)
		return this -> white.apply(move, this -> black);
	return this -> black.apply(move, this -> white);
}

void Board::print() {
	std::ostringstream board;
	board << "  abcdefgh  " << std::endl << std::endl;
	for(int i = 7; i >= 0; i--) {
		board << i + 1 << " ";
		for(int j = 0; j < 8; j++) {
			std::string piece, whitePiece, blackPiece;
			whitePiece = this -> white.getPieceName(i, j);
			blackPiece = this -> black.getPieceName(i, j);
			piece = whitePiece == "" ? blackPiece : whitePiece; 
			board << (piece == "" ? " " : piece);
		}
		board << " " << (8 - i) << std::endl;
	}
	board << std::endl << "  abcdefgh  " << std::endl;
	std::string b = board.str();
	std::reverse(b.begin(), b.end());
	std::cout << board.str();
}