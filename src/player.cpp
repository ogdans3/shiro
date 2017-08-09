#include "piece.h"
#include "pieceType.h"
#include "move.h"

class Player {
	std::vector<Piece *> pawns;
	std::vector<Piece *> rooks;
	std::vector<Piece *> knights;
	std::vector<Piece *> bishops;
	std::vector<Piece *> queens;
	std::vector<Piece *> kings;
	std::vector<Piece *> getPieces(Type::PieceType);
	std::vector<Piece *> getPieces(Type::PieceType, int, int);
	bool isWhite;

	public:
		void init(bool white);
		std::string getPieceName(int, int);
		Piece* getPiece(int, int);
		std::pair<int, int> apply(Move*, Player*);
		void newPiece(Type::PieceType, int, int);
		void reset();
		void dispose();
		std::vector<Piece *> getAllPieces();
};

void Player::reset() {
	this -> pawns = std::vector<Piece*>{};
	this -> rooks = std::vector<Piece*>{};
	this -> knights = std::vector<Piece*>{};
	this -> bishops = std::vector<Piece*>{};
	this -> queens = std::vector<Piece*>{};
	this -> kings = std::vector<Piece*>{};
	this -> init(this -> isWhite);
}

void Player::newPiece(Type::PieceType type, int row, int col) {
	Piece* piece = new Piece();
	piece -> init(type, row, col);
	switch(type) {
		case Type::PieceType::PAWN:
			this -> pawns.push_back(piece);
			break;
		case Type::PieceType::ROOK:
			this -> rooks.push_back(piece);
			break;
		case Type::PieceType::KNIGHT:
			this -> knights.push_back(piece);
			break;
		case Type::PieceType::BISHOP:
			this -> bishops.push_back(piece);
			break;
		case Type::PieceType::QUEEN:
			this -> queens.push_back(piece);
			break;
		case Type::PieceType::KING:
			this -> kings.push_back(piece);
			break;
	}
}

void Player::init(bool white) {
	this -> isWhite = white;

	int backrow = this -> isWhite ? 0 : 7;
	int pawnrow = this -> isWhite ? 1 : 6;

	//Initialise the first 8 pawns
	for(int i = 0; i < 8; i++) {
		this -> newPiece(Type::PieceType::PAWN, pawnrow, i);
	}

	//Initialize rooks
	this -> newPiece(Type::PieceType::ROOK, backrow, 0);
	this -> newPiece(Type::PieceType::ROOK, backrow, 7);

	//Initialize knights
	this -> newPiece(Type::PieceType::KNIGHT, backrow, 1);
	this -> newPiece(Type::PieceType::KNIGHT, backrow, 6);
	
	//Initialize bishops
	this -> newPiece(Type::PieceType::BISHOP, backrow, 2);
	this -> newPiece(Type::PieceType::BISHOP, backrow, 5);

	//Initialize king and queen
	this -> newPiece(Type::PieceType::QUEEN, backrow, 3);
	this -> newPiece(Type::PieceType::KING, backrow, 4);
}

std::vector<Piece*> Player::getAllPieces() {
	std::vector<Piece*> pieces = this -> pawns;

	pieces.insert(pieces.end(), this -> rooks.begin(), this -> rooks.end());
	pieces.insert(pieces.end(), this -> knights.begin(), this -> knights.end());
	pieces.insert(pieces.end(), this -> bishops.begin(), this -> bishops.end());
	pieces.insert(pieces.end(), this -> queens.begin(), this -> queens.end());
	pieces.insert(pieces.end(), this -> kings.begin(), this -> kings.end());
	return pieces;
}

std::string Player::getPieceName(int row, int col) {
	Piece* piece = this -> getPiece(row, col);
	if(piece == NULL)
		return "";
	return piece -> getName(); 
}
Piece* Player::getPiece(int row, int col) {
	for(Piece* piece: this -> getAllPieces()) {
		if(piece -> on(row, col) && piece -> getInPlay())
			return piece;
	}
	return NULL;
}

std::vector<Piece*> Player::getPieces(Type::PieceType type) {
	switch(type) {
		case Type::PieceType::PAWN:
			return this -> pawns;
		case Type::PieceType::ROOK:
			return this -> rooks;
		case Type::PieceType::KNIGHT:
			return this -> knights;
		case Type::PieceType::BISHOP:
			return this -> bishops;
		case Type::PieceType::QUEEN:
			return this -> queens;
		case Type::PieceType::KING:
			return this -> kings;
	}
	return std::vector<Piece*>{};
}

std::vector<Piece*> Player::getPieces(Type::PieceType type, int row, int col) {
	std::vector<Piece*> pieces = this -> getPieces(type);
	std::vector<Piece*> filtered;
	for(Piece* piece: pieces) {
		if(piece -> onEither(row, col) && piece -> getInPlay())
			filtered.push_back(piece);
	}
	return filtered;
}

std::pair<int, int> Player::apply(Move* move, Player* other) {
	if(move -> endOfGame())
		return std::make_pair(-1, -1);
	int moveIndex = move -> index;
	if(move -> kingSideCastle) {
		std::vector<Piece*> pieces = this -> getPieces(Type::PieceType::KING);
		std::vector<Piece*> rooks = this -> getPieces(Type::PieceType::ROOK);

		auto pair = std::make_pair(pieces[0] -> getRow(), pieces[0] -> getCol());
		if(this -> isWhite) {
			for(Piece* rook: rooks) {
				if(rook -> getRow() == 0 && rook -> getCol() == 7) {
					pieces[0] -> apply(0, 6, moveIndex);
					rook -> apply(0, 5, moveIndex);
				}
			}
		}else {
			for(Piece* rook: rooks) {
				if(rook -> getRow() == 7 && rook -> getCol() == 7) {
					pieces[0] -> apply(7, 6, moveIndex);
					rook -> apply(7, 5, moveIndex);
				}
			}

		}
		return pair;
	}
	if(move -> queenSideCastle) {
		std::vector<Piece*> pieces = this -> getPieces(Type::PieceType::KING);
		std::vector<Piece*> rooks = this -> getPieces(Type::PieceType::ROOK);

		auto pair = std::make_pair(pieces[0] -> getRow(), pieces[0] -> getCol());
		if(this -> isWhite) {
			for(Piece* rook: rooks) {
				if(rook -> getRow() == 0 && rook -> getCol() == 0) {
					pieces[0] -> apply(0, 2, moveIndex);
					rook -> apply(0, 3, moveIndex);
				}
			}
		}else {
			for(Piece* rook: rooks) {
				if(rook -> getRow() == 7 && rook -> getCol() == 0) {
					pieces[0] -> apply(7, 2, moveIndex);
					rook -> apply(7, 3, moveIndex);
				}
			}

		}
		return pair;
	}

	std::vector<Piece* > pieces = this -> getPieces(move -> piece);
	if(move -> ambiguous) {
		pieces = this -> getPieces(move -> piece, move -> fromRow, move -> fromCol);
	}
	for(Piece* piece: pieces) {
		if(!(piece -> getInPlay()))
			continue;
		bool canMove = piece -> canMoveTo(move -> toRow, move -> toCol, this -> isWhite,this -> getAllPieces(), other -> getAllPieces(), moveIndex);
		//piece -> print();
		//std::cout << move.str << std::endl;
		//std::cout << "Canmove: " << canMove << std::endl;
		if(canMove){
			if(move -> capture) {
				Piece* capturedPiece = other -> getPiece(move -> toRow, move -> toCol);
				if(capturedPiece == NULL) {
					if(this -> isWhite)
						capturedPiece = other -> getPiece(move -> toRow - 1, move -> toCol);
					else
						capturedPiece = other -> getPiece(move -> toRow + 1, move -> toCol);
				}
				capturedPiece -> setInPlay(false);
			}
			std::pair<int, int> pair = std::make_pair(piece -> getRow(), piece -> getCol());
			piece -> apply(move -> toRow, move -> toCol, moveIndex);
			if(move -> promotion) {
				Type::PieceType promotedTo = move -> promotedTo;
				this -> newPiece(promotedTo, move -> toRow, move -> toCol);	
				piece -> setInPlay(false);
			}
			return pair;
		};
	}
}

void Player::dispose() {
	std::vector<Piece*> pieces = this -> getAllPieces();
	for(int i = 0; i < pieces.size(); i++) {
		pieces[i] -> dispose();
		delete pieces[i];
	}
}