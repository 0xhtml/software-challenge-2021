#include <cmath>
#include "Evaluation.h"
#include "GameState.h"
#include "Pieces.h"

#define sq(n) pow(n, 2)

int Evaluation::evaluate(GameState gameState) {
    int evaluation = 0;

    for (int color = 0; color < COLOR_COUNT; ++color) {
        for (int piece = 0; piece < PIECE_COUNT; ++piece) {
            if (gameState.deployedPieces[color][piece]) {
                int value = floor(sqrt(sq(PIECE_BOUNDS[piece][0]) + sq(PIECE_BOUNDS[piece][1])));
                if (piece > 8) {
                    value += 5 * 2;
                } else if (piece > 3) {
                    value += 4 * 2;
                } else if (piece > 1) {
                    value += 3 * 2;
                } else if (piece > 0) {
                    value += 2 * 2;
                } else {
                    value += 1 * 2;
                }
                color % 2 == gameState.turn % 2 ? (evaluation += value) : (evaluation -= value);
            }
        }
    }

    return evaluation;
}
