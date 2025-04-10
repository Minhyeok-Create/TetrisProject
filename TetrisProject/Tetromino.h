#pragma once

enum TetrominoType {
	O, I, T, L, J, S, Z
};

class Tetromino {
public:
	char shape[4][4];
	int x, y;
	TetrominoType type;

	Tetromino();
	void rotate();

};