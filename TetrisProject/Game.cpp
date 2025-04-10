#include "Game.h"
#include <conio.h>
#include <windows.h>

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
		board.draw(current);
		current.y++;
		if (current.y > 17) current.y = 17;
		Sleep(300);

	}

}