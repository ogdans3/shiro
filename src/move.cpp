#include "pieceType.h"

class Move{
	bool capture;
	bool check;
	bool checkmate;
	bool promotion;

	Type::PieceType promotedTo;
	Type::PieceType piece;
	int row [8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int col [8] = {0, 0, 0, 0, 0, 0, 0, 0};	

	void parse(std::string);
	void parseName(std::string &);
	void parseMove(std::string);
	void parseIsCheck(std::string &);
	void parseIsCheckMate(std::string &);
	void parseIsCapture(std::string &);
	void parseIsPromotion(std::string &);

	public:
		void init(std::string);
};


void Move::parseName(std::string & move) {
	std::string pieceName;
	if(std::isupper((unsigned char) move[0])) {
		pieceName = std::string(1, move[0]);
		move = move.substr(1);
	}else {
		pieceName = "P";
	}
	this -> piece = Type::get(pieceName);
}

void Move::parseMove(std::string move) {
	std::cout << "Not implemented" << std::endl;
	std::cout << move << std::endl;
}

void Move::parseIsCheck(std::string & move) {
	this -> check = move[move.size() - 1] == '+';
	if(this -> check)
		move = move.substr(0, move.size() - 1);
}
void Move::parseIsCheckMate(std::string & move) {
	this -> checkmate = move[move.size() - 1] == '#';
	if(this -> checkmate)
		move = move.substr(0, move.size() - 1);
}
void Move::parseIsCapture(std::string & move) {
	auto pos = move.find('x');
	if(pos != std::string::npos) {
		this -> capture = true;
		move.erase(pos, pos + 1);
	}
}
void Move::parseIsPromotion(std::string & move) {
	this -> promotion = move[move.size() - 2] == '=';
	if(this -> promotion){
		this -> promotedTo = Type::get(std::string(1, move[move.size() - 1]));
		move = move.substr(0, move.size() - 2);
	}
}

void Move::parse(std::string move) {
	this -> parseName(move);

	this -> parseIsCheck(move);
	this -> parseIsCheckMate(move);
	this -> parseIsCapture(move);
	this -> parseIsPromotion(move);

	this -> parseMove(move);

	int row [8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int col [8] = {0, 0, 0, 0, 0, 0, 0, 0};	

}

void Move::init(std::string move) {
	std::cout << move << std::endl;
	this -> parse(move);
}