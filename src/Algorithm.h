#pragma once

#include <chrono>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "Evaluation.h"
#include "GameState.h"
#include "Types.h"

enum TranspositionType {
    EXACT, ALPHA, BETA
};

struct Transposition {
    TranspositionType type;
    int depth;
    int score;
};

class Algorithm {
private:
    Evaluation evaluation{};
    std::unordered_map<U64, Transposition> transpositions{};
    U64 history[PIECE_COUNT][ROTATION_COUNT][FLIPPED_COUNT][BOARD_SIZE][BOARD_SIZE]{};
    bool timeout;

    bool checkTimeout(Time start);

    std::vector<Move> sortedPossibleMoves(GameState &gameState) const;

    int alphaBeta(GameState &gameState, int depth, int alpha, int beta, Time start);

    Move alphaBetaRoot(GameState &gameState, int depth, int alpha, int beta, Time start);

public:
    Algorithm();

    Move iterativeDeepening(GameState &gameState, Time start);
};
