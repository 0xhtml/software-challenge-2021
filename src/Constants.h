#define BOARD_SIZE 20
#define BOARD_MAX BOARD_SIZE - 1

#define COORD_COUNT 2

#define PIECE_COORD_COUNT 5
#define PIECE_COORD_MAX PIECE_COORD_COUNT - 1

#define PIECE(pieces, move) pieces[move.piece][move.rotation][move.flipped]

#define PIECE_COUNT 21

const unsigned char PIECES[PIECE_COUNT][PIECE_COORD_COUNT + 1][COORD_COUNT] = {
        {{1, 1}, {0, 0}},
        {{2, 2}, {0, 0}, {1, 0}},
        {{3, 3}, {0, 0}, {0, 1}, {1, 1}},
        {{3, 2}, {0, 0}, {0, 2}, {0, 1}},
        {{4, 2}, {0, 0}, {0, 3}, {0, 1}, {0, 2}},
        {{4, 3}, {0, 0}, {0, 2}, {1, 2}, {0, 1}},
        {{4, 4}, {0, 0}, {0, 1}, {1, 0}, {1, 1}},
        {{4, 3}, {0, 0}, {1, 1}, {2, 0}, {1, 0}},
        {{4, 4}, {0, 0}, {1, 0}, {1, 1}, {2, 1}},
        {{5, 2}, {0, 0}, {0, 4}, {0, 1}, {0, 2}, {0, 3}},
        {{5, 4}, {0, 0}, {0, 1}, {2, 0}, {2, 1}, {1, 1}},
        {{5, 3}, {0, 0}, {0, 3}, {1, 3}, {0, 1}, {0, 2}},
        {{5, 3}, {0, 0}, {0, 2}, {2, 2}, {0, 1}, {1, 2}},
        {{5, 4}, {0, 0}, {1, 0}, {1, 1}, {0, 2}, {0, 1}},
        {{5, 5}, {0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 2}},
        {{5, 4}, {2, 0}, {0, 1}, {1, 2}, {2, 1}, {1, 1}},
        {{5, 4}, {0, 1}, {1, 0}, {2, 1}, {1, 2}, {1, 1}},
        {{5, 4}, {0, 0}, {1, 0}, {1, 2}, {2, 2}, {1, 1}},
        {{5, 3}, {1, 0}, {0, 1}, {1, 3}, {1, 1}, {1, 2}},
        {{5, 3}, {0, 0}, {2, 0}, {1, 2}, {1, 0}, {1, 1}},
        {{5, 4}, {0, 1}, {1, 0}, {1, 1}, {3, 0}, {2, 0}}
};

struct Move {
    unsigned char color, piece, rotation, flipped, x, y;
};

#define COLOR_COUNT 4
#define ROTATION_COUNT 4
#define FLIPPED_COUNT 2

#define DEBUG
#ifdef DEBUG

#include <iostream>

#define PRINT_MOVE(move) std::cout << std::to_string(move.color) << ", " << std::to_string(move.piece) << ", " << std::to_string(move.rotation) << ", " << std::to_string(move.flipped) << ", " << std::to_string(move.x) << ", " << std::to_string(move.y) << std::endl
#define PRINT_BOARD(gameState) std::cout << std::endl; for (unsigned char x = 0; x < BOARD_SIZE; ++x) { for (unsigned char y = 0; y < BOARD_SIZE; ++y) { std::cout << (gameState->boardGet(y, x) ? std::to_string(gameState->boardGet(y, x)) : "·")  << " "; } std::cout << std::endl; } std::cout << std::endl
#else
#define PRINT_MOVE(move)
#define PRINT_BOARD(gameState)
#endif