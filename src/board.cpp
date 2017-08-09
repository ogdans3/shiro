#include "player.cpp"
#include <sstream>      // std::ostringstream

#include "move.h"

class Board {
	Player white;
	Player black;
	public:
		void init();
		void print();
		std::pair<int, int> apply(Move);
		void reset();
		std::vector<Piece*> getAllPieces();
};

void Board::init() {
	this -> white.init(true);
	this -> black.init(false);
}

std::vector<Piece*> Board::getAllPieces() {
	std::vector<Piece*> pieces;
	std::vector<Piece*> white = this -> white.getAllPieces();
	std::vector<Piece*> black = this -> black.getAllPieces();

	pieces.insert(pieces.end(), white.begin(), white.end());
	pieces.insert(pieces.end(), black.begin(), black.end());
	return pieces;
}

std::pair<int, int> Board::apply(Move move) {
	if(move.whiteMove)
		return this -> white.apply(move, this -> black);
	return this -> black.apply(move, this -> white);
}

void Board::reset() {
	this -> white.reset();
	this -> black.reset();
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
	std::cout << board.str();
}