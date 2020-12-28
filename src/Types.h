#ifndef SOFTWARE_CHALLENGE_2021_TYPES_H
#define SOFTWARE_CHALLENGE_2021_TYPES_H

typedef u_int8_t U8;
typedef u_int32_t U32;
typedef u_int64_t U64;

struct Move {
    U8 color, piece, rotation, flipped, x, y;
};

struct Transposition {
    int bound, depth, score, bestMoveId;
};

struct MoveScorePair {
    Move move;
    int score;
};

#endif
