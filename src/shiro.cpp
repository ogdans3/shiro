#include <iostream>
#include <vector>

#include "../lib/eigen/Eigen/Dense"
#include "../lib/eigen/Eigen/Core"

#include "pgn-parser.cpp"
#include "network.cpp"

std::vector<std::pair<Eigen::VectorXd, Eigen::VectorXd> > getData() {
std::vector<std::vector<double> > dataset = {
	{2.7810836,2.550537003,0},
	{1.465489372,2.362125076,0},
	{3.396561688,4.400293529,0},
	{1.38807019,1.850220317,0},
	{3.06407232,3.005305973,0},
	{7.627531214,2.759262235,1},
	{5.332441248,2.088626775,1},
	{6.922596716,1.77106367,1},
	{8.675418651,-0.242068655,1},
	{7.673756466,3.508563011,1}};

	std::vector<std::pair<Eigen::VectorXd, Eigen::VectorXd> > data;

	for(uint i = 0; i < dataset.size(); i++) {
		Eigen::VectorXd x1(layerSizes[0]);
		Eigen::VectorXd y1(layerSizes[numLayers - 1]);
		x1 << dataset[i][0], dataset[i][1];
		y1 << dataset[i][2];
		data.push_back(std::make_pair(x1, y1));
	}
	return data;
}

int main(int argc, char const *argv[]) {
	std::vector<Game> games = parse("./data/games/A00.pgn");
	return 0;
	auto weights = genWeights(layerSizes[0]);
	train(weights, getData(), 1000);
	return 0;
}
