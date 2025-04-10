#include "Board.h"
#include <iostream>
#include <windows.h>

Board::Board() {
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			grid[i][j] = ' ';
}

void Board::clearScreen() {
	system("cls");
}

void Board::draw(const Tetromino& t, int score) {
	clearScreen();

	char display[20][10];
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			display[i][j] = grid[i][j];

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (t.shape[i][j] == '#')
				display[t.y + i][t.x + j] = '#';

	for (int i = 0; i < 20; ++i) {
		std::cout << "|";
		for (int j = 0; j < 10; ++j)
			std::cout << display[i][j];
		std::cout << "|" << std::endl;
	}

	std::cout << "Á¡¼ö : " << score << std::endl;
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

