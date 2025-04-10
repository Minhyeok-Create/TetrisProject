#include "Game.h"
#include <conio.h>
#include <windows.h>
#include <iostream>

void Game::run() {
	while (true) {
		if (_kbhit()) {
			char key = _getch();
			if (key == 75) {
				if (current.x > 0)
					current.x--;
			} else if (key == 77) {
				if (current.x < 8)
					current.x++;
			}
			else if (key == 27) {
				break;
			}
		}
		Tetromino next = current;
		next.y++;

		if (!board.checkCollision(next)) {
			current.y++;
		}
		else {
			board.placeTetromino(current);
			int lines = board.clearFullLines();
			switch (lines) {
			case 1: score += 100; break;
			case 2: score += 300; break;
			case 3: score += 500; break;
			case 4: score += 800; break;
			default: break;
			}

			current = Tetromino();

			if (board.checkCollision(current)) {
				board.draw(current,score);
				std::cout << "게임 오버!" << std::endl;
				break;
			}
		}
	

		board.draw(current, score);
		Sleep(300);

	}

}