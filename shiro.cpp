#include <iostream>
#include <vector>

#include "lib/eigen/Eigen/Dense"

const std::vector<int> layerSizes = std::vector<int>{
	2,
	1
};

typedef std::vector<Eigen::MatrixXd> Weights;

void print(Weights weights) {
	for(uint i = 0; i < weights.size(); i++) {
		std::cout << "Layer: " << i << std::endl << weights[i] << std::endl;
	}
}

Weights genWeights(int inputSize) {
	std::vector<Eigen::MatrixXd> weights;
	for(uint i = 0; i < layerSizes.size() - 1; i++) {
		int rows = layerSizes[i + 1];
		int cols = layerSizes[i];
		weights.push_back(Eigen::MatrixXd::Random(rows, cols));
	}
	return weights;
}

Eigen::VectorXd forward(Weights weights, Eigen::VectorXd inputs) {
	print(weights);
	for(uint i = 0; i < weights.size(); i++) {
		inputs = weights[i] * inputs;
	}
	return inputs;
}

Weights train() {
	auto weights = genWeights(layerSizes[0]);
	Eigen::VectorXd input(layerSizes[0]);
	input << 1, 1;
	std::cout << forward(weights, input) << std::endl;
	return weights;
}

int main(int argc, char const *argv[]) {
	train();
	return 0;
}
