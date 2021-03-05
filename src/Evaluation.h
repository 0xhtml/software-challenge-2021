#pragma once

#include <vector>
#include "Constants.h"
#include "Types.h"
#include "GameState.h"

class Evaluation {
private:
    std::vector<PositionValuePair> positionsSortedByDistanceToMiddle{};

    int evaluateDistanceToMiddle(GameState gameState, int color);

    void evaluateFields(GameState gameState, int color, int &value);

public:
    int pieceEvaluation[PIECE_COUNT]{};

    Evaluation();

    int evaluate(GameState gameState);
};
