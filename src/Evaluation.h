#pragma once

#include <vector>

#include "Constants.h"
#include "GameState.h"
#include "Types.h"

struct PositionValuePair {
    U8 x, y;
    int value;
};

class Evaluation {
private:
    std::vector<PositionValuePair> positionsSortedByDistanceToMiddle{};

    int distanceToMiddle(const GameState &gameState, const int color) const;

    int getSide(const GameState &gameState, const int team) const;

    void evaluateSpace(const GameState &gameState, const int team, int &value) const;

public:
    int pieceSize[PIECE_COUNT]{};

    Evaluation();

    int evaluate(const GameState &gameState) const;
};
