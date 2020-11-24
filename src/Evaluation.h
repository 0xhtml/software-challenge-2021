#ifndef SOFTWARE_CHALLENGE_2021_EVALUATION_H
#define SOFTWARE_CHALLENGE_2021_EVALUATION_H

#include "GameState.h"
#include "Hash.h"

struct CacheEntry {
    unsigned long int hash;
    int evaluation;
    unsigned char control;
};

class Evaluation {
private:
    Hash hash{};
    CacheEntry cache[1000]{};
public:
    int evaluate(GameState gameState);
};

#endif
