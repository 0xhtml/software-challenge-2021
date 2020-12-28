#include "Evaluation.h"
#include "Pieces.h"

int Evaluation::evaluate(Move move) {
    int score = PIECE_SCORES[move.piece] * 14 * BOARD_SIZE;

    for (int x = 0; x < PIECE_SIZE; ++x) {
        for (int y = 0; y < PIECE_SIZE; ++y) {
            if (PIECES[move.piece][move.rotation][move.flipped][x] & 1 << y) {
                score += COORD_SCORES[move.x + x][move.y + y];
            }
        }
    }

    return score;
}
