#pragma once
#include "Tetromino.h"

class Board {
public:
	char grid[20][10];

	Board();
	void draw(const Tetromino& t);
	void clearScreen();
};
