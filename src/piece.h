#ifndef PIECE
#define PIECE
#include "pieceType.h"

class Piece {
	bool inPlay = true;
	Type::PieceType piece;
	int row = 0;
	int col = 0;
	int enPassantableMoveIndex = -1;
	public:
		void init(std::string, int, int);
		void init(Type::PieceType, int, int);
		bool on(int, int);
		bool onEither(int, int);
		bool canMoveTo(int, int);
		void print();
		std::string getName();
		void setInPlay(bool);
		bool getInPlay();
		bool isOfType(Type::PieceType);
		void apply(int, int, int);
		bool isEnPaassantable(int, int, int, bool);
		int getRow(){return this -> row;};
		int getCol(){return this -> col;};
		bool canMoveTo(int, int, bool, std::vector<Piece*>, std::vector<Piece*>, int);
		Type::PieceType getType();
		void dispose();
};

#include "piece.cpp"
#endif