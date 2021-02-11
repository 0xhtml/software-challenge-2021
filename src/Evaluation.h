#pragma once

#include "Constants.h"
#include "Types.h"

const int PIECE_SCORES[PIECE_COUNT]{-15, 5, 7, 8, 11, 10, 9, 10, 10, 14, 12, 13, 12, 12, 12, 12, 12, 12, 13, 12, 13};

class Evaluation {
public:
    static int evaluate(Move move);
};
