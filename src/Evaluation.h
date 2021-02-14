#pragma once

#include <vector>
#include "Constants.h"
#include "Types.h"
#include "GameState.h"

#define WIN_SCORE 100

class Evaluation {
private:
    std::vector<CoordValuePair> coordsSortedByDistanceToMiddle{};

    int evaluateDistanceToMiddle(GameState gameState, int color);

public:
    int pieceEvaluation[PIECE_COUNT]{};

    Evaluation();

    int evaluate(GameState gameState);
};
