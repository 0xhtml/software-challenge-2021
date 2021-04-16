#pragma once

#include <vector>
#include "Constants.h"
#include "Types.h"
#include "GameState.h"

class Evaluation {
private:
    std::vector<PositionValuePair> positionsSortedByDistanceToMiddle{};

    int evaluateDistanceToMiddle(const GameState &gameState, int color) const;

    void evaluatePieceSize(const GameState &gameState, int color, int &value) const;

public:
    int pieceSize[PIECE_COUNT]{};

    Evaluation();

    int evaluate(const GameState &gameState) const;
};
