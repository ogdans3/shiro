#ifndef PIECETYPE
#define PIECETYPE

#include <algorithm>
#include <string> 

namespace Type {
	enum PieceType {
		QUEEN,
		BISHOP,
		KNIGHT,
		ROOK,
		PAWN,
		KING,
		INVALID
	};

	std::string getName(PieceType piece) {
		switch(piece) {
			case PieceType::KING:
				return "K";
			case PieceType::QUEEN:
				return "Q";
			case PieceType::BISHOP:
				return "B";
			case PieceType::KNIGHT:
				return "N";
			case PieceType::ROOK:
				return "R";
			case PieceType::PAWN:
				return "P";
			default:
				return "I"; //Invalid
		}
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