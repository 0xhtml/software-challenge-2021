#pragma once

#include <chrono>
#include <unordered_map>
#include "Evaluation.h"
#include "GameState.h"
#include "Types.h"

class Algorithm {
private:
    Evaluation evaluation{};
    std::chrono::time_point<std::chrono::system_clock> start;
    std::unordered_map<U64, Transposition> transpositions;
    bool timeout;

    bool checkTimeout();

    std::vector<Move> sortedPossibleMoves(GameState &gameState) const;

    int alphaBeta(GameState &gameState, int depth, int alpha, int beta);

    Move alphaBetaRoot(GameState &gameState, int depth, int alpha, int beta);

public:
    Move iterativeDeepening(GameState &gameState);
};
