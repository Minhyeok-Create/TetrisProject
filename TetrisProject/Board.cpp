#include "Board.h"
#include <SFML/Graphics.hpp>

Board::Board() {
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			grid[i][j] = ' ';
}


void Board::draw(sf::RenderWindow& window, const Tetromino& t, const Tetromino& ghost, int score, int level) {
	
	const float blockSize = 30.f;
	sf::RectangleShape block(sf::Vector2f(blockSize, blockSize));
	block.setOutlineThickness(1.f);
	block.setOutlineColor(sf::Color::Black);


	for (int y = 0; y < 20; ++y) {
		for (int x = 0; x < 10; ++x) {
			if (grid[y][x] == '#') {
				block.setFillColor(sf::Color::Blue);
				block.setPosition(sf::Vector2f(x * blockSize, y * blockSize));
				window.draw(block);
			}
		}
	}
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (ghost.shape[i][j] == '#') {
				int x = ghost.x + j;
				int y = ghost.y + i;
				if (y >= 0 && y < 20 && x >= 0 && grid[y][x] == ' ') {
					block.setFillColor(sf::Color(200, 200, 200, 100));
					block.setPosition(sf::Vector2f(x * blockSize, y * blockSize));
					window.draw(block);
				}
			}
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (t.shape[i][j] == '#') {
				int x = t.x + j;
				int y = t.y + i;
				block.setFillColor(sf::Color::Cyan);
				block.setPosition(sf::Vector2f(x * blockSize, y * blockSize));
				window.draw(block);
			}

	
}
	void Board::placeTetromino(const Tetromino & t) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				if (t.shape[i][j] == '#')
					grid[t.y + i][t.x + j] = '#';
	}
	bool Board::checkCollision(const Tetromino& t) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (t.shape[i][j] == '#') {
					int newY = t.y + i;
					int newX = t.x + j;

					if (newY >= 20 || newX < 0 || newX >= 10) return true;
					if (grid[newY][newX] == '#') return true;
				}
			}
		}
		return false;
	}
	int Board::clearFullLines() {
		int linesCleared = 0;

		for (int i = 19; i >= 0; --i) {
			bool full = true;
			for (int j = 0; j < 10; ++j) {
				if (grid[i][j] != '#') {
					full = false;
					break;
				}
			}
			if (full) {
				linesCleared++;

				for (int row = i; row > 0; --row) {
					for (int col = 0; col < 10; ++col) {
						grid[row][col] = grid[row - 1][col];
					}
				}
				for (int col = 0; col < 10; ++col) {
					grid[0][col] = ' ';
				}
				i++;
			}
		}
		return linesCleared;
	}

