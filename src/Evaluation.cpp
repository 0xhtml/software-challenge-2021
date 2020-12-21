#include "Evaluation.h"

int Evaluation::evaluate(GameState gameState) {
    int evaluation = 0;
    for (unsigned char x = 0; x < BOARD_SIZE; ++x) {
        for (unsigned char y = 0; y < BOARD_SIZE; ++y) {
            int value = gameState.boardGet(x, y);
            if (!value) continue;
            (value - 1) % 2 == gameState.getTurn() % 2 ? (evaluation++) : (evaluation--);
        }
    }

    return evaluation;
}
