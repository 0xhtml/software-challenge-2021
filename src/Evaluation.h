#ifndef SOFTWARE_CHALLENGE_2021_EVALUATION_H
#define SOFTWARE_CHALLENGE_2021_EVALUATION_H

#include "Constants.h"
#include "Types.h"

const int PIECE_SCORES[PIECE_COUNT]{2, 5, 7, 8, 11, 10, 9, 10, 10, 14, 12, 13, 12, 12, 12, 12, 12, 12, 13, 12, 13};
const int COORD_SCORES[BOARD_SIZE][BOARD_SIZE]{{1, 2, 2, 3, 4, 4,  4,  5,  5,  5,  5,  5,  5,  4,  4,  4, 3, 2, 2, 1},
                                               {2, 2, 3, 4, 4, 5,  5,  6,  6,  6,  6,  6,  6,  5,  5,  4, 4, 3, 2, 2},
                                               {2, 3, 4, 5, 5, 6,  6,  7,  7,  7,  7,  7,  7,  6,  6,  5, 5, 4, 3, 2},
                                               {3, 4, 5, 5, 6, 7,  7,  8,  8,  8,  8,  8,  8,  7,  7,  6, 5, 5, 4, 3},
                                               {4, 4, 5, 6, 7, 7,  8,  8,  9,  9,  9,  9,  8,  8,  7,  7, 6, 5, 4, 4},
                                               {4, 5, 6, 7, 7, 8,  9,  9,  10, 10, 10, 10, 9,  9,  8,  7, 7, 6, 5, 4},
                                               {4, 5, 6, 7, 8, 9,  10, 10, 11, 11, 11, 11, 10, 10, 9,  8, 7, 6, 5, 4},
                                               {5, 6, 7, 8, 8, 9,  10, 11, 12, 12, 12, 12, 11, 10, 9,  8, 8, 7, 6, 5},
                                               {5, 6, 7, 8, 9, 10, 11, 12, 12, 13, 13, 12, 12, 11, 10, 9, 8, 7, 6, 5},
                                               {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5},
                                               {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5},
                                               {5, 6, 7, 8, 9, 10, 11, 12, 12, 13, 13, 12, 12, 11, 10, 9, 8, 7, 6, 5},
                                               {5, 6, 7, 8, 8, 9,  10, 11, 12, 12, 12, 12, 11, 10, 9,  8, 8, 7, 6, 5},
                                               {4, 5, 6, 7, 8, 9,  10, 10, 11, 11, 11, 11, 10, 10, 9,  8, 7, 6, 5, 4},
                                               {4, 5, 6, 7, 7, 8,  9,  9,  10, 10, 10, 10, 9,  9,  8,  7, 7, 6, 5, 4},
                                               {4, 4, 5, 6, 7, 7,  8,  8,  9,  9,  9,  9,  8,  8,  7,  7, 6, 5, 4, 4},
                                               {3, 4, 5, 5, 6, 7,  7,  8,  8,  8,  8,  8,  8,  7,  7,  6, 5, 5, 4, 3},
                                               {2, 3, 4, 5, 5, 6,  6,  7,  7,  7,  7,  7,  7,  6,  6,  5, 5, 4, 3, 2},
                                               {2, 2, 3, 4, 4, 5,  5,  6,  6,  6,  6,  6,  6,  5,  5,  4, 4, 3, 2, 2},
                                               {1, 2, 2, 3, 4, 4,  4,  5,  5,  5,  5,  5,  5,  4,  4,  4, 3, 2, 2, 1}};

class Evaluation {
public:
    static int evaluate(Move move);
};

#endif
