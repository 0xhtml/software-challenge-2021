#ifndef SOFTWARE_CHALLENGE_2021_TYPES_H
#define SOFTWARE_CHALLENGE_2021_TYPES_H

typedef u_int8_t U8;
typedef u_int32_t U32;
typedef u_int64_t U64;

struct Move {
    unsigned char color, piece, rotation, flipped, x, y;
};

struct Transposition {
    int bound, depth, value;
};

struct MoveValuePair {
    Move move;
    int value;
};

#endif
