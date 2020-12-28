#ifndef SOFTWARE_CHALLENGE_2021_TYPES_H
#define SOFTWARE_CHALLENGE_2021_TYPES_H

typedef unsigned char U8;
typedef unsigned int U32;
typedef unsigned long int U64;

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
