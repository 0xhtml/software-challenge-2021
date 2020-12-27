#include <cmath>
#include "Evaluation.h"
#include "GameState.h"
#include "Types.h"

int Evaluation::evaluateCoords(int x, int y) {
    int distX = BOARD_SIZE / 2 - x;
    int distY = BOARD_SIZE / 2 - y;
    return 14 - floor(sqrt(distX * distX + distY * distY));
}

int Evaluation::evaluate(GameState gameState) {
    int evaluation = 0;

    for (int color = 0; color < COLOR_COUNT; ++color) {
        for (int piece = 0; piece < PIECE_COUNT; ++piece) {
            if (gameState.deployedPieces[color][piece]) {
                int value = PIECE_EVALUATION[piece] * 14 * BOARD_SIZE;
                color % 2 == gameState.turn % 2 ? (evaluation += value) : (evaluation -= value);
            }
        }
    }

    for (int x = 0; x < BOARD_SIZE; ++x) {
        U32 own = gameState.board[gameState.turn % 4 + 1][x] | gameState.board[(gameState.turn + 2) % 4 + 1][x];
        U32 opp = gameState.board[(gameState.turn + 1) % 4 + 1][x] | gameState.board[(gameState.turn + 3) % 4 + 1][x];
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if (own & 1 << y) {
                evaluation += evaluateCoords(x, y);
            } else if (opp & 1 << y) {
                evaluation -= evaluateCoords(x, y);
            }
        }
    }

    return evaluation;
}
