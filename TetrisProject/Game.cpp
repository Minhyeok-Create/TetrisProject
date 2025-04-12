#include "Game.h"
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <chrono>

void Game::run() {
	auto lastFall = std::chrono::steady_clock::now();

	while (true) {
		if (_kbhit()) {
			char key = _getch();
			Tetromino temp = current;
			if (key == 75 && current.x >0) {
				temp.x--;
				if (!board.checkCollision(temp))
					current.x--;
			} else if (key == 77 && current.x < 7) {
				temp.x++;
				if (!board.checkCollision(temp))
					current.x++;
			}
			else if (key == 'z' || key == 72) {
				temp.rotate();
				if (!board.checkCollision(temp))
					current = temp;
			}
			else if (key == 27) {
				break;
			}
		}
		auto now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFall);
		if (duration.count() > 300) {
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
					board.draw(current, score);
					std::cout << "게임 오버!" << std::endl;
					break;
				}
			}
			lastFall = now;
		}

		board.draw(current, score);
		Sleep(10);

	}

}