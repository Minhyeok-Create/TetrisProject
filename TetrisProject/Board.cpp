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

void Board::draw(const Tetromino& t) {
	clearScreen();

	char display[20][10];
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			display[i][j] = grid[i][j];

	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 2; ++j)
			if (t.shape[i][j] == '#')
				display[t.y + i][t.x + j] = '#';

	for (int i = 0; i < 20; ++i) {
		std::cout << "|";
		for (int j = 0; j < 10; ++j)
			std::cout << display[i][j];
		std::cout << "|" << std::endl;

	}
}