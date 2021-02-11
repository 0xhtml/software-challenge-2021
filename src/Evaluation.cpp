#include "Evaluation.h"

int Evaluation::evaluate(Move move) {
    return PIECE_SCORES[move.piece];
}
