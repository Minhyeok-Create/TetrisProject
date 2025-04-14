#pragma once
#include "Board.h"
#include "Tetromino.h"
#include <SFML/Graphics.hpp>
#include <chrono>

class Game {
public:
	Game();
	void run();

private:
	enum class GameState {
		Menu,
		Playing,
		GameOver,
		Paused
	};
	GameState state = GameState::Menu;

	bool isPaused = false;
	bool gameStarted = false;
	bool pKeyHeld = false;

	Board board;
	Tetromino current, next, hold;
	bool hasHold = false;
	bool holdUsedThisTurn = false;

	int score = 0;
	int level = 1;
	int speed = 500;
	bool isGameOver = false;

	sf::Font font;
	sf::Text scoreText, levelText;
	std::wstring bonusMessage;
	std::chrono::steady_clock::time_point bonusTimer;
	bool showBonus = false;

	// 내부 함수
	void drawNext(sf::RenderWindow& window);
	void drawHold(sf::RenderWindow& window);
	void drawBonus(sf::RenderWindow& window);
	void drawPause(sf::RenderWindow& window);
	void resetGame();
	void handleFall();
};
