const int numLayers = 9;
const int layerSizes [numLayers] = {2, 40, 100, 200, 1000, 200, 100, 40, 1};
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
