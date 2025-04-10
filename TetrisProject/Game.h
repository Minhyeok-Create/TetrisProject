#pragma once
#include "Board.h"
#include "Tetromino.h"
class Game {
public:
	void run();
private:
	Board board;
	Tetromino current;
};