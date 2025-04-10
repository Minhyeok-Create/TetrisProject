#pragma once
#include "Tetromino.h"

class Board {
public:
	char grid[20][10];

	Board();
	void draw(const Tetromino& t, int score);
	void clearScreen();

	void placeTetromino(const Tetromino& t);
	bool checkCollision(const Tetromino& t);
	int clearFullLines();
	
	
};

