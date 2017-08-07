#ifndef STUDENT_H
#define STUDENT_H

#include <algorithm>
#include <string> 

namespace Type {
	enum PieceType {
		KING,
		QUEEN,
		BISHOP,
		KNIGHT,
		ROOK,
		PAWN,
		INVALID
	};

	std::string getName(PieceType piece) {
		if(piece == PieceType::KING)
			return "K";
		if(piece == PieceType::QUEEN)
			return "Q";
		if(piece == PieceType::BISHOP)
			return "B";
		if(piece == PieceType::KNIGHT)
			return "N";
		if(piece == PieceType::ROOK)
			return "R";
		if(piece == PieceType::PAWN)
			return "P";
	}

	PieceType get(std::string piece) {
		std::transform(piece.begin(), piece.end(), piece.begin(), ::tolower);

		if(piece == "k" || piece == "king")
			return PieceType::KING;

		if(piece == "q" || piece == "queen")
			return PieceType::QUEEN;

		if(piece == "b" || piece == "bishop")
			return PieceType::BISHOP;

		if(piece == "n" || piece == "knight")
			return PieceType::KNIGHT;

		if(piece == "r" || piece == "rook")
			return PieceType::ROOK;

		if(piece == "" || piece == "p" || piece == "pawn")
			return PieceType::PAWN;
		return PieceType::INVALID;
	}
}

#endif