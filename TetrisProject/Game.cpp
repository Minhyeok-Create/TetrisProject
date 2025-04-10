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
			board.clearFullLines();
			current = Tetromino();

			if (board.checkCollision(current)) {
				board.draw(current);
				std::cout << "게임 오버!" << std::endl;
				break;
			}
		}

		board.draw(current);
		Sleep(300);

	}

}