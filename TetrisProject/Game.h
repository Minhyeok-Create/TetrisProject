#pragma once
#include "Board.h"
#include "Tetromino.h"
#include <SFML/Graphics/Text.hpp>
class Game {
public:
	Game();
	void run();
private:
	Board board;
	Tetromino current;
	int score = 0;
	int level = 1;
	int speed = 500;	
	bool isGameOver = false;

	sf::Font font;
	sf::Text scoreText;
	sf::Text levelText;
};