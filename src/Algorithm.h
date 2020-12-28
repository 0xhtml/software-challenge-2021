#ifndef SOFTWARE_CHALLENGE_2021_ALGORITHM_H
#define SOFTWARE_CHALLENGE_2021_ALGORITHM_H

#include <chrono>
#include <unordered_map>
#include "GameState.h"
#include "Types.h"

class Algorithm {
private:
    std::chrono::time_point<std::chrono::system_clock> start;
    std::unordered_map<U64, Transposition> transpositions;
    bool timeout;

    int alphaBeta(GameState gameState, int depth, int alpha, int beta);

    MoveScorePair alphaBetaRoot(GameState gameState, int depth, int alpha, int beta);

public:
    MoveScorePair iterativeDeepening(GameState gameState);
};

#endif
