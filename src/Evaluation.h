#ifndef SOFTWARE_CHALLENGE_2021_EVALUATION_H
#define SOFTWARE_CHALLENGE_2021_EVALUATION_H

#include "Constants.h"
#include "GameState.h"

const int PIECE_EVALUATION[PIECE_COUNT]{2, 5, 7, 8, 11, 10, 9, 10, 10, 14, 12, 13, 12, 12, 12, 12, 12, 12, 13, 12, 13};

class Evaluation {
public:
    static int evaluateCoords(int x, int y);

    static int evaluate(GameState gameState);
};

#endif
