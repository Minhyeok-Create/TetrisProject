#pragma once
#include "Tetromino.h"

class Board {
public:
	char grid[20][10];

	Board();
	void draw(const Tetromino& t, const Tetromino& ghost, int score, int level);
	void clearScreen();

	void placeTetromino(const Tetromino& t);
	bool checkCollision(const Tetromino& t);
	int clearFullLines();
	
	
};

