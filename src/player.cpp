#include "piece.cpp"

class Player {
	Piece pieces [16];
	bool isWhite;
	public:
		void init(bool white);
		std::string getPiece(int, int);
};

void Player::init(bool white) {
	this -> isWhite = white;

	int backrow = this -> isWhite ? 0 : 7;
	int pawnrow = this -> isWhite ? 1 : 6;

	//Initialise the first 8 pawns
	for(int i = 0; i < 8; i++) {
		this -> pieces[i].init("P", pawnrow, i);
	}

	//Initialize rooks
	this -> pieces[8].init("R", backrow, 0);
	this -> pieces[9].init("R", backrow, 7);

	//Initialize knights
	this -> pieces[10].init("N", backrow, 1);
	this -> pieces[11].init("N", backrow, 6);

	//Initialize bishops
	this -> pieces[12].init("B", backrow, 2);
	this -> pieces[13].init("B", backrow, 5);

	//Initialize king and queen
	this -> pieces[14].init("Q", backrow, 3);
	this -> pieces[15].init("K", backrow, 4);
}

std::string Player::getPiece(int row, int col) {
	for(Piece piece: this -> pieces) {
		if(piece.on(row, col))
			return piece.getName();
	}
	return "";
}