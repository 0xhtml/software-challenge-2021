#ifndef SOFTWARE_CHALLENGE_2021_ALGORITHM_H
#define SOFTWARE_CHALLENGE_2021_ALGORITHM_H

#include "GameState.h"
#include "Evaluation.h"

class Algorithm {
private:
    Evaluation evaluation{};
    int initDepth = 2;
    Move bestMove{};

    int alphaBeta(GameState gameState, int depth, int alpha, int beta);

public:
    Move run(GameState gameState);
};

#endif
