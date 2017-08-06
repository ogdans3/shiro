#include "player.cpp"
#include <sstream>      // std::ostringstream

class Board {
	Player white;
	Player black;
	public:
		void init();
		void print();
};

void Board::init() {
	this -> white.init(true);
	this -> black.init(false);
}

void Board::print() {
	std::ostringstream board;
	board << "  abcdefgh  " << std::endl << std::endl;
	for(int i = 0; i < 8; i++) {
		board << i + 1 << " ";
		for(int j = 0; j < 8; j++) {
			std::string piece, whitePiece, blackPiece;
			whitePiece = this -> white.getPiece(i, j);
			blackPiece = this -> black.getPiece(i, j);
			piece = whitePiece == "" ? blackPiece : whitePiece; 
			board << (piece == "" ? " " : piece);
		}
		board << " " << (8 - i) << std::endl;
	}
	board << std::endl << "  abcdefgh  " << std::endl;
	std::cout << board.str();
}