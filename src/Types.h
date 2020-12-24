#ifndef SOFTWARE_CHALLENGE_2021_TYPES_H
#define SOFTWARE_CHALLENGE_2021_TYPES_H

#define U8 u_int8_t
#define U32 u_int32_t
#define U64 u_int64_t

struct Move {
    unsigned char color, piece, rotation, flipped, x, y;
};

struct Transposition {
    int bound, depth, value;
};

#endif
