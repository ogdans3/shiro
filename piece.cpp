class Piece {
	std::string name;
	bool inPlay;
	int row [8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int col [8] = {0, 0, 0, 0, 0, 0, 0, 0};
	public:
		void init(std::string, int, int);
		bool on(int, int);
		void print();
		std::string getName();
};

void Piece::init(std::string name, int row, int col) {
	this -> name = name;
	this -> row[row] = 1;
	this -> col[col] = 1;
}

std::string Piece::getName() {
	return this -> name;
}

bool Piece::on(int row, int col) {
	return this -> row[row] == 1 && this -> col[col] == 1;
}

void Piece::print() {
	std::cout << this -> name << ", ";
	for(int i = 0; i < 8; i++) {
		if(row[i] == 1)
			std::cout << i;
	}
	std::cout << ", ";
	for(int i = 0; i < 8; i++) {
		if(col[i] == 1)
			std::cout << i;
	}
	std::cout << std::endl;
}