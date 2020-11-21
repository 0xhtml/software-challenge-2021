#ifndef SOFTWARE_CHALLENGE_2021_ALGORITHM_H
#define SOFTWARE_CHALLENGE_2021_ALGORITHM_H

#include "GameState.h"

class Algorithm {
private:
    GameState gameState{};
    int initDepth = 2;
    Move bestMove{};

    int alphaBeta(int depth, int alpha, int beta);

public:
    Move run(GameState gameState1);
};

#endif