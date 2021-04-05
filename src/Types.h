#pragma once

#include <stdint.h>

typedef uint8_t U8;
typedef uint32_t U32;
typedef uint64_t U64;

struct Move {
    U8 color, piece, rotation, flipped, x, y;
};

struct PositionValuePair {
    U8 x, y;
    int value;
};

struct Transposition {
    int bound, depth, score;
};
