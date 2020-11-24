#ifndef SOFTWARE_CHALLENGE_2021_CONSTANTS_H
#define SOFTWARE_CHALLENGE_2021_CONSTANTS_H

#define BOARD_SIZE 20
#define BOARD_MAX BOARD_SIZE - 1

#define COORD_COUNT 2

#define PIECE_COORD_COUNT 5
#define PIECE_COORD_MAX PIECE_COORD_COUNT - 1

#define PIECE(pieces, move) pieces[move.piece][move.rotation][move.flipped]

#define PIECE_COUNT 21

struct Piece {
    unsigned char coord_count;
    unsigned char diagonal_coord_count;
    unsigned char coords[PIECE_COORD_COUNT][COORD_COUNT];
    unsigned char bounds[COORD_COUNT];
};

const Piece PIECES[PIECE_COUNT] = {
        {1, 1, {{0, 0}}},
        {2, 2, {{0, 0}, {1, 0}}},
        {3, 3, {{0, 0}, {0, 1}, {1, 1}}},
        {3, 2, {{0, 0}, {0, 2}, {0, 1}}},
        {4, 2, {{0, 0}, {0, 3}, {0, 1}, {0, 2}}},
        {4, 3, {{0, 0}, {0, 2}, {1, 2}, {0, 1}}},
        {4, 4, {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
        {4, 3, {{0, 0}, {1, 1}, {2, 0}, {1, 0}}},
        {4, 4, {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
        {5, 2, {{0, 0}, {0, 4}, {0, 1}, {0, 2}, {0, 3}}},
        {5, 4, {{0, 0}, {0, 1}, {2, 0}, {2, 1}, {1, 1}}},
        {5, 3, {{0, 0}, {0, 3}, {1, 3}, {0, 1}, {0, 2}}},
        {5, 3, {{0, 0}, {0, 2}, {2, 2}, {0, 1}, {1, 2}}},
        {5, 4, {{0, 0}, {1, 0}, {1, 1}, {0, 2}, {0, 1}}},
        {5, 5, {{0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 2}}},
        {5, 4, {{2, 0}, {0, 1}, {1, 2}, {2, 1}, {1, 1}}},
        {5, 4, {{0, 1}, {1, 0}, {2, 1}, {1, 2}, {1, 1}}},
        {5, 4, {{0, 0}, {1, 0}, {1, 2}, {2, 2}, {1, 1}}},
        {5, 3, {{1, 0}, {0, 1}, {1, 3}, {1, 1}, {1, 2}}},
        {5, 3, {{0, 0}, {2, 0}, {1, 2}, {1, 0}, {1, 1}}},
        {5, 4, {{0, 1}, {1, 0}, {1, 1}, {3, 0}, {2, 0}}}
};

struct Move {
    unsigned char color, piece, rotation, flipped, x, y;
};

#define COLOR_COUNT 4
#define ROTATION_COUNT 4
#define FLIPPED_COUNT 2

#define PRINT_BOARD(gameState) std::cout << std::endl; for (unsigned char y = 0; y < BOARD_SIZE; ++y) { for (unsigned char x = 0; x < BOARD_SIZE; ++x) { std::cout << (gameState.boardGet(x, y) ? std::to_string(gameState.boardGet(x, y)) : "·")  << " "; } std::cout << std::endl; } std::cout << std::endl

// TODO: Calculate most significant board position
#define MOST_SIGNIFICANT_X 0
#define MOST_SIGNIFICANT_Y 1

#endif
