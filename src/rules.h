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
		if(abs(toRow - fromRow) == 1 && abs(toCol - fromCol) == 2)
			return true;
		if(abs(toRow - fromRow) == 2 && abs(toCol - fromCol) == 1)
			return true;
		return false;
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

	inline bool pieceInTheWay(int toRow, int toCol, int fromRow, int fromCol, std::vector<Piece*> pieces,
								std::vector<std::pair<int, int> > notOn, int newRow, int newCol) {
		
		int hRow = std::max(toRow, fromRow);
		int lRow = std::min(toRow, fromRow);
		int hCol = std::max(toCol, fromCol);
		int lCol = std::min(toCol, fromCol);

		if(hRow > newRow && newRow > lRow && newCol == hCol && newCol == lCol) 
			return true;
		if(hCol > newCol && newCol > lCol && newRow == hRow && newRow == lRow)
			return true;
		if(	abs(newRow - fromRow) == abs(newCol - fromCol) &&
			hRow > newRow && hCol > newCol &&
			newRow > lRow && newCol > lCol)
			return true;

		for(Piece* piece: pieces) {
			if(!(piece -> getInPlay()))
				continue;
			int row = piece -> getRow();
			int col = piece -> getCol();
			
			bool mayContinue = true;
			for(auto pos: notOn) {
				if(pos.first == row && pos.second == col) {
					mayContinue = false;
					break;
				}
			}
			if(!mayContinue)
				continue;

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

	inline bool enPassant(int toRow, int toCol, std::vector<Piece*> pieces, int moveIndex, bool white) {
		for(Piece* piece: pieces) {
			if(!(piece -> getInPlay()))
				continue;
			if(piece -> getType() != Type::PieceType::PAWN)
				continue;
			if(piece -> isEnPaassantable(toRow, toCol, moveIndex, white)) {
				return true;
			}
		}
		return false;
	}

	inline bool movementLegal(Type::PieceType piece, int toRow, int toCol, int fromRow, int fromCol, bool white) {
		switch(piece){
			case Type::PieceType::PAWN:
				return 	rules::pawnMovementLegal(toRow, toCol, fromRow, fromCol, white);
			case Type::PieceType::ROOK:
				return 	rules::rookMovementLegal(toRow, toCol, fromRow, fromCol);
			case Type::PieceType::KNIGHT:
				return 	rules::knightMovementLegal(toRow, toCol, fromRow, fromCol);
			case Type::PieceType::BISHOP:
				return 	rules::bishopMovementLegal(toRow, toCol, fromRow, fromCol);
			case Type::PieceType::QUEEN:
				return 	rules::queenMovementLegal(toRow, toCol, fromRow, fromCol);
			case Type::PieceType::KING:
				return 	rules::kingMovementLegal(toRow, toCol, fromRow, fromCol);
		}
	}

	inline bool movesIntoCheck(std::vector<Piece*> friendly, std::vector<Piece*> hostiles, bool white,
								int fromRow, int fromCol, int toRow, int toCol, int moveIndex) {
		Piece* king;
		for(Piece* piece: friendly) {
			if(piece -> getType() == Type::PieceType::KING){
				king = piece;
				break;
			}
		}
		int kRow = king -> getRow();
		int kCol = king -> getCol();
		for(Piece* piece: hostiles) {
			if(!(piece -> getInPlay()))
				continue;
			int row = piece -> getRow();
			int col = piece -> getCol();
			if(row == toRow && col == toCol)
				continue;
			if(	rules::movementLegal(piece -> getType(), kRow, kCol, row, col, !white) &&
				!rules::pieceInTheWay(kRow, kCol, row, col, friendly, std::vector<std::pair<int ,int> > {std::make_pair(fromRow, fromCol)}, toRow, toCol) && 
				!rules::pieceInTheWay(kRow, kCol, row, col, hostiles, std::vector<std::pair<int ,int> > {std::make_pair(fromRow, fromCol)}, toRow, toCol)) {
				if(piece -> getType() == Type::PieceType::PAWN) {
					if((	
							(	
								col != kCol && rules::pieceOnTarget(kRow, kCol, hostiles) || 
								col != kCol && rules::enPassant(kRow, kCol, hostiles, moveIndex, !white)
							)
							|| 
							(
								col == kCol && !rules::pieceOnTarget(kRow, kCol, friendly)
								&& 
								col == kCol && !rules::pieceOnTarget(kRow, kCol, hostiles) 
							)
						)){
						return true;
					}
				}else{
					return true;
				}
				//return true;
			}
		}
		return false;
	}


	inline bool canMoveTo(Type::PieceType piece, int toRow, int toCol, bool white, int fromRow, int fromCol, std::vector<Piece*> friendly, std::vector<Piece*> hostile, int moveIndex) {
		switch(piece){
			case Type::PieceType::PAWN:
				return 	rules::pawnMovementLegal(toRow, toCol, fromRow, fromCol, white) &&
						(	
							(	
								fromCol != toCol && rules::pieceOnTarget(toRow, toCol, hostile) || 
								fromCol != toCol && rules::enPassant(toRow, toCol, hostile, moveIndex, white)
							)
							|| 
							(
								fromCol == toCol && !rules::pieceOnTarget(toRow, toCol, friendly)
								&& 
								fromCol == toCol && !rules::pieceOnTarget(toRow, toCol, hostile) 
							)
						) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, hostile) &&
						!rules::movesIntoCheck(friendly, hostile, white, fromRow, fromCol, toRow, toCol, moveIndex);
			case Type::PieceType::ROOK:
				return 	rules::rookMovementLegal(toRow, toCol, fromRow, fromCol) &&
						!rules::pieceOnTarget(toRow, toCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, hostile) &&
						!rules::movesIntoCheck(friendly, hostile, white, fromRow, fromCol, toRow, toCol, moveIndex);
			case Type::PieceType::KNIGHT:
				return 	rules::knightMovementLegal(toRow, toCol, fromRow, fromCol) &&
						!rules::pieceOnTarget(toRow, toCol, friendly) &&
						!rules::movesIntoCheck(friendly, hostile, white, fromRow, fromCol, toRow, toCol, moveIndex);
			case Type::PieceType::BISHOP:
				return 	rules::bishopMovementLegal(toRow, toCol, fromRow, fromCol) &&
						!rules::pieceOnTarget(toRow, toCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, hostile) &&
						!rules::movesIntoCheck(friendly, hostile, white, fromRow, fromCol, toRow, toCol, moveIndex);
			case Type::PieceType::QUEEN:
				return 	rules::queenMovementLegal(toRow, toCol, fromRow, fromCol) &&
						!rules::pieceOnTarget(toRow, toCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, friendly) &&
						!rules::pieceInTheWay(toRow, toCol, fromRow, fromCol, hostile) &&
						!rules::movesIntoCheck(friendly, hostile, white, fromRow, fromCol, toRow, toCol, moveIndex);
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