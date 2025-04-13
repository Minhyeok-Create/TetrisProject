#pragma once
#include "Tetromino.h"
#include <SFML/Graphics.hpp>

class Board {
public:
	char grid[20][10];

	Board();
	void draw(sf::RenderWindow& window, const Tetromino& t, const Tetromino& ghost, int score, int level);

	void placeTetromino(const Tetromino& t);
	bool checkCollision(const Tetromino& t);
	int clearFullLines();
	
	
};

