#include "pieceType.h"

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

void Move::parseIsKingSideCastle(std::string & move) {
	if(move == "O-O") {
		this -> kingSideCastle = true;
		move = "";
	}
}

void Move::parseIsQueenSideCastle(std::string & move) {
	if(move == "O-O-O") {
		this -> queenSideCastle = true;
		move = "";
	}
}

void Move::parseMove(std::string & move) {
	if(move.size() <= 1){
	    throw std::invalid_argument("Move size was <= 1 must atleast contain row and column of target position, " + move);
	}

	int toRow = (move.back() - '1');
	move.pop_back();
	int toCol = (move.back() - 'a');
	move.pop_back();
	if(toRow < 0 || toRow > 7) {
	    throw std::invalid_argument("Last row was not bounded to between 1 and 8, " + move);		
	}
	if(toCol < 0 || toCol > 7) {
	    throw std::invalid_argument("Last col was not bounded to between a and h, " + move);
	}
	this -> toRow = toRow;
	this -> toCol = toCol;
	this -> parseAmbiguity(move);
}

//TODO: Test this properly
void Move::parseAmbiguity(std::string & move) {
	// The notation is Nab7 or N2b7 or Na2b7. All cases must be handled
	if(move.size() == 0)
		return;
	this -> ambiguous = true;

	int fromRow = (move.back() - '1');
	if(fromRow > -1 && fromRow < 8) {
		this -> fromRow = fromRow;
		return;
	}

	int fromCol = (move.back() - 'a');
	move.pop_back();
	if(fromCol < 0 || fromCol > 7) {
	    throw std::invalid_argument("From col was not bounded to between 1 and 8, " + move);		
	}
	this -> fromCol = fromCol;

	//This has never been tested
	if(move.size() == 0)
		return;
	fromRow = (move.back() - '1');
	move.pop_back();
	if(fromRow < 0 || fromRow > 7) {
	    throw std::invalid_argument("From row was not bounded to between 1 and 8, " + move);		
	}
	this -> fromRow = fromRow;
}

void Move::parseIsCheck(std::string & move) {
	this -> check = move[move.size() - 1] == '+';
	if(this -> check)
		move.pop_back();
}
void Move::parseIsCheckMate(std::string & move) {
	this -> checkmate = move[move.size() - 1] == '#';
	if(this -> checkmate)
		move.pop_back();
}
void Move::parseIsCapture(std::string & move) {
	this -> capture = move.find('x') != std::string::npos;
	if(this -> capture) {
		move.erase(std::remove(move.begin(), move.end(), 'x'), move.end());
	}
}
void Move::parseIsPromotion(std::string & move) {
	this -> promotion = move[move.size() - 2] == '=';
	if(this -> promotion){
		this -> promotedTo = Type::get(std::string(1, move[move.size() - 1]));
		move.pop_back();
		move.pop_back();
	}
}

bool Move::endOfGame() {
	if(this -> tie || this -> win || this -> loss) {
		return true;
	}
	return false;
}


void Move::parseIsTie(std::string & move) {
	if(move == "0-0" || move == "1/2-1/2") {
		this -> tie = true;
		move = "";
	}
}

void Move::parseIsWin(std::string & move) {
	if(move == "1-0") {
		this -> win = true;
		move = "";		
	}
}

void Move::parseIsLoss(std::string & move) {
	if(move == "0-1") {
		this -> loss = true;
		move = "";		
	}
}

void Move::parse(std::string move) {
	this -> str = std::string(move);
	this -> parseIsTie(move);
	this -> parseIsWin(move);
	this -> parseIsLoss(move);
	if(this -> tie || this -> win || this -> loss)
		return;

	this -> parseIsCheck(move);
	this -> parseIsCheckMate(move);
	this -> parseIsCapture(move);
	this -> parseIsPromotion(move);

	this -> parseIsKingSideCastle(move);
	this -> parseIsQueenSideCastle(move);
	if(this -> kingSideCastle || this -> queenSideCastle)
		return;

	this -> parseName(move);

	this -> parseMove(move);

	int row [8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int col [8] = {0, 0, 0, 0, 0, 0, 0, 0};	

}

Move::Move(std::string move, bool white, int index) {
	this -> parse(move);
	this -> index = index;
	this -> whiteMove = white;
}