#ifndef MOVE
#define MOVE

class Move{
	void parse(std::string);
	void parseName(std::string &);
	void parseMove(std::string &);
	void parseAmbiguity(std::string &);
	void parseIsTie(std::string &);
	void parseIsWin(std::string &);
	void parseIsLoss(std::string &);
	void parseIsCheck(std::string &);
	void parseIsCheckMate(std::string &);
	void parseIsCapture(std::string &);
	void parseIsPromotion(std::string &);
	void parseIsKingSideCastle(std::string &);
	void parseIsQueenSideCastle(std::string &);

	public:
		Move(std::string, bool, int);
		bool endOfGame();
		bool whiteMove = false;
		bool capture = false;
		bool check = false;
		bool checkmate = false;
		bool promotion = false;
		bool kingSideCastle = false;
		bool queenSideCastle = false;
		bool ambiguous = false;

		bool tie = false;
		bool win = false;
		bool loss = false;

		int index = -1;
		std::string str = "";

		int toRow = -1;
		int toCol = -1;
		int fromRow = -1;
		int fromCol = -1;

		Type::PieceType promotedTo = Type::PieceType::INVALID;
		Type::PieceType piece = Type::PieceType::INVALID;
};

#include "move.cpp"

#endif