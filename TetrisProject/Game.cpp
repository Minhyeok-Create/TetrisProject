#define NOMINMAX
#include "Game.h"
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <chrono>
#include <algorithm>

void Game::run() {
	auto lastFall = std::chrono::steady_clock::now();

	while (true) {
		if (_kbhit()) {
			char key = _getch();
			Tetromino temp = current;
			if (key == 75) {
				temp.x--;
				if (!board.checkCollision(temp))
					current.x--;
			} else if (key == 77 ) {
				temp.x++;
				if (!board.checkCollision(temp))
					current.x++;
			}
			else if (key == 'z' || key == 72) {
				Tetromino rotated = current;
				rotated.rotate();

				bool rotatedOK = false;
				for (int dx = -1; dx <= 1; ++dx) {
					Tetromino temp = rotated;
					temp.x += dx;
					if (!board.checkCollision(temp)) {
						current = temp;
						rotatedOK = true;
						break;
					}
				}
				if (!rotatedOK) {

				}
			}
			else if (key == 80) {
				Tetromino dropped = current;
				while (true) {
					Tetromino next = dropped;
					next.y++;
					if (board.checkCollision(next)) break;
					dropped.y++;
				}
				current = dropped;
				board.placeTetromino(current);

			}
			else if (key == 27) {
				break;
			}
		}
		auto now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFall);
		if (duration.count() > speed) {
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
				level = score / 1000 + 1;
				int fallSpeed = 500 - (level - 1) * 40;
				speed = std::max(100, fallSpeed);

				current = Tetromino();

				if (board.checkCollision(current)) {
					board.draw(current, score, level);
					std::cout << "게임 오버!" << std::endl;
					break;
				}
			}
			lastFall = std::chrono::steady_clock::now();
			continue;
		}

		board.draw(current, score, level);
		Sleep(10);

	}

}