#ifndef SOFTWARE_CHALLENGE_2021_HASH_H
#define SOFTWARE_CHALLENGE_2021_HASH_H

#include "GameState.h"

class Hash {
private:
    unsigned long int table[BOARD_SIZE][BOARD_SIZE][COLOR_COUNT]{};
public:
    Hash();

    unsigned long int hash(GameState gameState);
};

#endif
