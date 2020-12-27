#ifndef SOFTWARE_CHALLENGE_2021_HASH_H
#define SOFTWARE_CHALLENGE_2021_HASH_H

#include <algorithm>
#include "Constants.h"
#include "Types.h"

class Hash {
private:
    U64 table[BOARD_SIZE][BOARD_SIZE][COLOR_COUNT]{};
public:
    Hash();

    U64 hash(Move move);
};

#endif
