#ifndef SOFTWARE_CHALLENGE_2021_ALGORITHM_H
#define SOFTWARE_CHALLENGE_2021_ALGORITHM_H

#include <unordered_map>
#include <chrono>
#include "GameState.h"
#include "Hash.h"
#include "Types.h"

class Algorithm {
private:
    Hash hash{};
    std::chrono::time_point<std::chrono::system_clock> start;
    std::unordered_map<U64, Transposition> transpositions;
    bool timeout;

    int alphaBeta(GameState gameState, int depth, int alpha, int beta);

    MoveValuePair alphaBetaRoot(GameState gameState, int depth, int alpha, int beta);

public:
    MoveValuePair iterativeDeepening(GameState gameState);
};

#endif
