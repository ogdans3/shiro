#include <iostream>
#include <vector>

#include "lib/eigen/Eigen/Dense"
#include "lib/eigen/Eigen/Core"

const int numLayers = 3;
const int layerSizes [numLayers] = {2, 2, 1};
const double learningRate = 0.5;
Eigen::VectorXd deltas [numLayers] = {};

typedef std::vector<Eigen::MatrixXd> Weights;

void print(Weights weights) {
	for(uint i = 0; i < weights.size(); i++) {
		std::cout << "Layer: " << i << std::endl << weights[i] << std::endl;
	}
}

double sigmoid(double x) {
	return 1 / (1 + std::exp(-x));
}

double transfer_sigmoid(double x) {
	return x * (1 - x);
}

Weights genWeights(int inputSize) {
	Weights weights;
	for(uint i = 0; i < numLayers - 1; i++) {
		int rows = layerSizes[i + 1];
		int cols = layerSizes[i];
		weights.push_back(Eigen::MatrixXd::Random(rows, cols));
	}
	return weights;
}

void forward(Weights weights, Eigen::VectorXd inputs, std::vector<Eigen::VectorXd> &outputs) {
	outputs[0] = inputs;
	for(uint i = 0; i < weights.size(); i++) {
		inputs = (weights[i] * inputs).unaryExpr(&sigmoid);
		outputs[i + 1] = inputs;
	}
}

void backward(Weights weights, Eigen::VectorXd expected, std::vector<Eigen::VectorXd> outputs) {
	auto error = expected - outputs[numLayers - 1];
	deltas[numLayers - 1] = error;
//	deltas[numLayers - 1] = (error * outputs[numLayers - 1].unaryExpr(&transfer_sigmoid)).transpose();
	for(int i = numLayers - 2; i >= 0; i--) {
		auto err =  deltas[i + 1].transpose() * weights[i];
		deltas[i] = outputs[i].unaryExpr(&transfer_sigmoid).transpose().cwiseProduct(err);
	}
}

void update_weights(Weights &weights, std::vector<Eigen::VectorXd> outputs, Eigen::VectorXd input) {
	for(uint i = 0; i < outputs.size() - 1; i++) {
		auto grad = learningRate * (deltas[i + 1] * outputs[i].transpose()); 
		weights[i] += grad;
	}
}

Weights train(Weights weights, std::vector<std::pair<Eigen::VectorXd, Eigen::VectorXd> > data, int epochs) {
	std::vector<Eigen::VectorXd> outputs(numLayers);
	for(int q = 0; q < epochs; q++) {
		double error = 0;
		for(uint i = 0; i < data.size(); i++) {	
			Eigen::VectorXd input = data[i].first;
			Eigen::VectorXd expected = data[i].second;
			
			forward(weights, input, outputs);
			backward(weights, expected, outputs);
			update_weights(weights, outputs, input);

			error += (expected - outputs[numLayers - 1]).squaredNorm();
		}
		std::cout << "Epoch: " << q << ", " << "LR: " << learningRate << ", " << "Error: " << error << std::endl;
	}
	return weights;
}

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
	auto weights = genWeights(layerSizes[0]);
	train(weights, getData(), 20);
	return 0;
}
