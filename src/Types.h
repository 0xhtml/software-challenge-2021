#pragma once

typedef unsigned char U8;
typedef unsigned int U32;
typedef unsigned long int U64;

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
