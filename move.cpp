#include "pieceType.h"

class Move{
	bool capture;
	Type::PieceType piece;
	int row [8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int col [8] = {0, 0, 0, 0, 0, 0, 0, 0};	
	void parse(std::string);
	public:
		void init(std::string);
};

void Move::parse(std::string move) {
	Type::PieceType piece;
	int row [8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int col [8] = {0, 0, 0, 0, 0, 0, 0, 0};	

//	Type::get();
}

void Move::init(std::string move) {
	std::cout << move << std::endl;
}