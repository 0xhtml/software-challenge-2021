#define BOARD_SIZE 20
#define BOARD_MAX BOARD_SIZE - 1

#define PIECE_COORD_COUNT 5
#define PIECE_COORD_MAX PIECE_COORD_COUNT - 1
#define PIECE_X(pieces, move) pieces[move.piece][move.rotation][move.flipped][PIECE_COORD_COUNT]

#define PIECE_COUNT 21

//@formatter:off
const unsigned char PIECES[PIECE_COUNT][PIECE_COORD_COUNT][PIECE_COORD_COUNT] = {
        {{1}},         // MONO
        {{1}, {1}},    // DOMINO
        {{1, 1}, {1}}, // TRIO-L

        {{1, 1, 1}},      // TRIO-I
        {{1, 1, 1, 1}},   // TETRO-I
        {{1, 1, 1}, {1}}, // TETRO-L

        {{1, 1}, {1, 1}},      // TETRO-O
        {{1}, {1, 1}, {1}},    // TETRO-T
        {{1}, {1, 1}, {0, 1}}, // TETRO-Z

        {{1, 1, 1, 1, 1}},            // PENTO-I
        {{1, 1}, {0, 1}, {1, 1}},     // PENTO-U
        {{1, 1, 1, 1}, {0, 0, 0, 1}}, // PENTO-L

        {{1, 1, 1}, {0, 0, 1}, {0, 0, 1}}, // PENTO-V
        {{1, 1, 1}, {1, 1}},               // PENTO-P
        {{1, 1}, {0, 1, 1}, {0, 0, 1}},    // PENTO-W

        {{0, 1}, {0, 1, 1}, {1, 1}}, // PENTO-R
        {{0, 1}, {1, 1, 1}, {0, 1}}, // PENTO-X
        {{1}, {1, 1, 1}, {0, 0, 1}}, // PENTO-Z

        {{0, 1}, {1, 1, 1, 1}},     // PENTO-Y
        {{1}, {1, 1, 1}, {1}},      // PENTO-T
        {{0, 1}, {1, 1}, {1}, {1}}, // PENTO-S
};

//@formatter:on

struct Move {
    unsigned char color, piece, rotation, flipped, x, y;
};

#define COLOR_COUNT 4
#define ROTATION_COUNT 4
#define FLIPPED_COUNT 2

#define DEBUG
#ifdef DEBUG

#include <iostream>

#define PRINT_PIECE(piece) for (unsigned char x = 0; x < PIECE_COORD_COUNT; ++x) { for (unsigned char y = 0; y < PIECE_COORD_COUNT; ++y) { std::cout << piece[y][x] << " "; } std::cout << std::endl; } std::cout << std::endl
#define PRINT_MOVE(move) std::cout << std::to_string(move.color) << ", " << std::to_string(move.piece) << ", " << std::to_string(move.rotation) << ", " << std::to_string(move.flipped) << ", " << std::to_string(move.x) << ", " << std::to_string(move.y) << std::endl
#define PRINT_BOARD(gameState) std::cout << std::endl; for (unsigned char x = 0; x < BOARD_SIZE; ++x) { for (unsigned char y = 0; y < BOARD_SIZE; ++y) { std::cout << (gameState->boardGet(y, x) ? std::to_string(gameState->boardGet(y, x)) : "·")  << " "; } std::cout << std::endl; } std::cout << std::endl
#else
#define PRINT_PIECE(piece)
#define PRINT_MOVE(move)
#define PRINT_BOARD(gameState)
#endif