#pragma once

#include "Constants.h"
#include "Types.h"

class Hash {
private:
    U64 table[BOARD_SIZE][BOARD_SIZE][COLOR_COUNT]{};
public:
    Hash();

    U64 hash(const Move &move) const;
};
