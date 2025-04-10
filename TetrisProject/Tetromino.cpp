#include "Tetromino.h"
#include <cstdlib>
#include <ctime>

Tetromino::Tetromino() {

    x = 3;
    y = 0;
    srand(time(nullptr));
    type = static_cast<TetrominoType>(rand() % 7);

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            shape[i][j] = ' ';

    switch (type) {
    case O:
        shape[1][1] = '#'; shape[1][2] = '#';
        shape[2][1] = '#'; shape[2][2] = '#';
        break;
    case I:
        shape[1][0] = '#'; shape[1][1] = '#';
        shape[1][2] = '#'; shape[1][3] = '#';
        break;
    case T:
        shape[1][1] = '#'; shape[1][0] = '#';
        shape[1][2] = '#'; shape[2][1] = '#';
        break;
    case L:
        shape[0][1] = '#'; shape[1][1] = '#';
        shape[2][1] = '#'; shape[2][2] = '#';
        break;
    case J:
        shape[0][1] = '#'; shape[1][1] = '#';
        shape[2][1] = '#'; shape[2][0] = '#';
        break;
    case S:
        shape[1][1] = '#'; shape[1][2] = '#';
        shape[2][0] = '#'; shape[2][1] = '#';
        break;
    case Z:
        shape[1][0] = '#'; shape[1][1] = '#';
        shape[2][1] = '#'; shape[2][2] = '#';
        break;
    }
}

void Tetromino::rotate() {
    char temp[4][4];

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
			temp[j][3 - i] = shape[i][j];

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			shape[i][j] = temp[i][j];
}