#include <iostream>
#include <vector>

#include "../lib/eigen/Eigen/Dense"
#include "../lib/eigen/Eigen/Core"

int linesDone = 0;
int skippedGames = 0;
#include "pgn-parser.cpp"

const int numLayers = 7;
const int layerSizes [numLayers] = {704, 800, 1000, 1000, 500, 100, 36};
const double learningRate = 0.1;
#include "network.cpp"

Game* game;
int games = 1;
std::vector<std::string> files;
int file = 0;

void log(int game, double lr, double error){
	std::cout << files[file] << " " << linesDone <<  " SkippedGames: " << skippedGames << " Game: " << game << ", LR: " << lr << ", Error: " << error << std::endl;
}

void log(int _linesRead, int _skippedGames) {
	linesDone = _linesRead;
	skippedGames = _skippedGames;
}

void dispose() {
	if(game == NULL)
		return;
	game -> dispose();
	delete game;
}

std::pair<Eigen::VectorXd, Eigen::VectorXd> generateData() {
	if(game == NULL) {
		std::cout << files[file];
		dispose();
		games ++;
		game = parseGame(files[file]);
		if(game == NULL){
			file ++;
			if(file > files.size() - 1) {
				throw std::out_of_range("All files finished");
			}
			throw std::out_of_range("File finished");
		}
	}

	std::vector<Piece*> pieces = game -> getState();
	Move* move = game -> getNextMove();
	if(move == NULL || move -> endOfGame()) {
		std::cout << files[file];
		dispose();
		games ++;
		game = parseGame(files[file]);
		if(game == NULL) {
			if(file >= files.size() - 1) {
				throw std::out_of_range("All files finished");
			}
			file ++;
			throw std::out_of_range("File finished");
		}
		throw std::runtime_error("Game finished");
	}
//	std::cout << files[file] << " Game: " << games << " Move: " << move -> index << ". " << move -> str << std::endl;

	Eigen::VectorXd input = Eigen::VectorXd::Zero((pieces.size()) * 22);
	int i = 0;
	for(Piece* piece: pieces) {
		int type = piece -> getType();
		int row = piece -> getRow();
		int col = piece -> getCol();
		int offset = 22 * i;
		input[offset + type] = 1;
		input[offset + 6 + row] = 1;
		input[offset + 6 + 8 + col] = 1;
		i ++;
	}
	
	//Create 16 vector elements for row, column of where the piece is
	//Create 16 vector elements for row, column of where the piece should be moved
	//Create 4 vector elements for promoting: queen, bishop, knight, rook
	Eigen::VectorXd out = Eigen::VectorXd::Zero(36);

	std::pair<int, int> from = game -> nextMove();
//	game -> print();
	out[from.first] = 1;
	out[8 + from.second] = 1;
	
	out[16 + move -> toRow] = 1;
	out[8 + 16 + move -> toCol] = 1;

	if(move -> promotion) {
		out[move -> promotedTo] = 1;
	}
	return std::make_pair(input, out);
}

int main(int argc, char const *argv[]) {
	for(int i = 1; i < argc; i++) {
		files.push_back(std::string(argv[i]));
	}
//	game = parseGame("data/games/test.pgn");
//	game -> play();
	auto weights = genWeights(layerSizes[0]);
	train(weights, generateData, log);
	return 0;
}
