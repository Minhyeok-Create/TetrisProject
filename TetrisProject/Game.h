#pragma once
#include "Board.h"
#include "Tetromino.h"
#include <SFML/Graphics/Text.hpp>
#include <chrono>

class Game {
public:
	Game();
	void run();
private:
	bool isPaused = false;
	bool gameStarted = false;
	bool pKeyHeld = false;

	Board board;
	Tetromino current;
	int score = 0;
	int level = 1;
	int speed = 500;	
	bool isGameOver = false;

	sf::Font font;
	sf::Text scoreText;
	sf::Text levelText;

	void drawNext(sf::RenderWindow& window);
	void drawHold(sf::RenderWindow& window);
	Tetromino next;

	std::wstring bonusMessage;
	std::chrono::steady_clock::time_point bonusTimer;
	bool showBonus = false;

	Tetromino hold;
	bool hasHold = false;
	bool holdUsedThisTurn = false;

};