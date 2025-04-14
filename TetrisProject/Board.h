#pragma once
#include "Tetromino.h"
#include <SFML/Graphics.hpp>
#include <optional>

class Board {
public:

	Board();
	void draw(sf::RenderWindow& window, const Tetromino& t, const Tetromino& ghost, int score, int level);

	void placeTetromino(const Tetromino& t);
	bool checkCollision(const Tetromino& t);
	int clearFullLines();
	
private:
	std::optional<TetrominoType>grid[20][10];
	
};
sf::Color getColorByType(TetrominoType type);

