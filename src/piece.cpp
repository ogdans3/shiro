#include "rules.h"

void Piece::init(std::string name, int row, int col) {
	this -> piece = Type::get(name);
	this -> row = row;
	this -> col = col;
}

void Piece::init(Type::PieceType piece, int row, int col) {
	this -> piece = piece;
	this -> row = row;
	this -> col = col;
}

std::string Piece::getName() {
	return Type::getName(this -> piece);
}

bool Piece::on(int row, int col) {
	return this -> row == row && this -> col == col;
}

bool Piece::onEither(int row, int col) {
	return this -> row == row || this -> col == col;
}

bool Piece::canMoveTo(int row, int col, bool white, std::vector<Piece*> friendly, std::vector<Piece*> hostile) {
	return rules::canMoveTo(this -> piece, row, col, white, this -> row, this -> col, friendly, hostile);
}

bool Piece::isOfType(Type::PieceType type) {
	return this -> piece == type;
}

void Piece::print() {
	std::cout << this -> getName() << ", ";
	for(int i = 0; i < 8; i++) {
		if(row == i)
			std::cout << i;
	}
	std::cout << ", ";
	for(int i = 0; i < 8; i++) {
		if(col == i)
			std::cout << i;
	}
	std::cout << ", " << this -> getInPlay() << std::endl;
}

void Piece::apply(int row, int col) {
	this -> row = row;
	this -> col = col;
}

void Piece::setInPlay(bool inPlay) {
	this -> inPlay = inPlay;
}
bool Piece::getInPlay() {
	return this -> inPlay;
}