#ifndef RULES
#define RULES
#include "piece.h"
#include "pieceType.h"
#include <algorithm>

namespace rules {
	inline bool pawnMovementLegal(int toRow, int toCol, int fromRow, int fromCol, bool white) {
		if(white) {
			return 
				(fromCol == toCol && fromRow == toRow - 1) || //Forward movement
				(fromCol == toCol && fromRow == toRow - 2) || //Double forward move
				(fromCol == toCol - 1 && fromRow == toRow - 1) ||
				(fromCol == toCol + 1 && fromRow == toRow - 1);
		}
		return 
			(fromCol == toCol && fromRow == toRow + 1) || //Forward movement
			(fromCol == toCol && fromRow == toRow + 2) || //Double forward move
			(fromCol == toCol - 1 && fromRow == toRow + 1) ||
			(fromCol == toCol + 1 && fromRow == toRow + 1);
	}

	inline bool rookMovementLegal(int toRow, int toCol, int fromRow, int fromCol) {
		return 
			fromRow == toRow ||
			fromCol == toCol;
	}

	inline bool knightMovementLegal(int toRow, int toCol, int fromRow, int fromCol) {
		return sqrt(pow(fromRow - toRow, 2) + pow(fromCol - toCol, 2)) == sqrt(pow(0 - 2, 2) + pow(1 - 2, 2));
	}

	inline bool bishopMovementLegal(int toRow, int toCol, int fromRow, int fromCol) {
		return abs(fromRow - toRow) == abs(fromCol - toCol);
	}

	inline bool queenMovementLegal(int toRow, int toCol, int fromRow, int fromCol) {
		return 
			rules::bishopMovementLegal(toRow, toCol, fromRow, fromCol) ||
			rules::rookMovementLegal(toRow, toCol, fromRow, fromCol);
	}

	inline bool kingMovementLegal(int toRow, int toCol, int fromRow, int fromCol) {
		return 
			(abs(fromRow - toRow) == 1 && abs(fromCol - toCol) == 0) ||
			(abs(fromCol - toCol) == 1 && abs(fromRow - toRow) == 0) ||
			(abs(fromCol - toCol) == 1 && abs(fromRow - toRow) == 1);
	}

	inline bool pieceOnTarget(int row, int col, std::vector<Piece*> pieces) {
		for(Piece* piece: pieces) {
			if(piece -> on(row, col) && piece -> getInPlay()) {
				return true;
			}
		}
		return false;
	}

	inline bool pieceInTheWay(int toRow, int toCol, int fromRow, int fromCol, std::vector<Piece*> pieces) {
		for(Piece* piece: pieces) {
			if(!(piece -> getInPlay()))
				continue;
			int row = piece -> getRow();
			int col = piece -> getCol();
			int hRow = std::max(toRow, fromRow);
			int lRow = std::min(toRow, fromRow);
			int hCol = std::max(toCol, fromCol);
			int lCol = std::min(toCol, fromCol);
			if(hRow > row && row > lRow && col == hCol && col == lCol) 
				return true;
			if(hCol > col && col > lCol && row == hRow && row == lRow)
				return true;
			if(	abs(row - fromRow) == abs(col - fromCol) &&
				hRow > row && hCol > col &&
				row > lRow && col > lCol)
				return true;
		}
		return false;
	}


	inline bool canMoveTo(Type::PieceType piece, int toRow, int toCol, bool white, int fromRow, int fromCol, std::vector<Piece*> friendly, std::vector<Piece*> hostile) {
		switch(piece){
			case Type::PieceType::PAWN:
				//TODO: Do we have to handle an passant?
				return 	rules::pawnMovementLegal(toRow, toCol, fromRow, fromCol, white) &&
						(	(fromCol != toCol && rules::pieceOnTarget(toRow, toCol, hostile))
							|| 
							(fromCol == toCol && !rules::pieceOnTarget(toRow, toCol, friendly)
							&& 
							fromCol == toCol && !rules::pieceOnTarget(toRow, toCol, hostile) )
						) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, hostile);
			case Type::PieceType::ROOK:
				return 	rules::rookMovementLegal(toRow, toCol, fromRow, fromCol) &&
						!rules::pieceOnTarget(toRow, toCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, hostile);
			case Type::PieceType::KNIGHT:
				return 	rules::knightMovementLegal(toRow, toCol, fromRow, fromCol) &&
						!rules::pieceOnTarget(toRow, toCol, friendly);
			case Type::PieceType::BISHOP:
				return 	rules::bishopMovementLegal(toRow, toCol, fromRow, fromCol) &&
						!rules::pieceOnTarget(toRow, toCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, hostile);
			case Type::PieceType::QUEEN:
				return 	rules::queenMovementLegal(toRow, toCol, fromRow, fromCol) &&
						!rules::pieceOnTarget(toRow, toCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, hostile);
			case Type::PieceType::KING:
				//TODO: Should also look for checks
				//TODO: Handle castling
				return 	rules::kingMovementLegal(toRow, toCol, fromRow, fromCol) &&
						!rules::pieceOnTarget(toRow, toCol, friendly);
		}
		return false;
	}
}


#endif