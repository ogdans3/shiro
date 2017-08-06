#include "pieceType.h"

class Move{
	bool capture;
	pieceType::PieceType piece;
	int row [8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int col [8] = {0, 0, 0, 0, 0, 0, 0, 0};	
	public:
		void init(std::string);
};


void Move::init(std::string move) {
	std::cout << "Move" << std::endl;
	std::cout << move << std::endl;
}