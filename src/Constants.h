#ifndef SOFTWARE_CHALLENGE_2021_CONSTANTS_H
#define SOFTWARE_CHALLENGE_2021_CONSTANTS_H

#define BOARD_SIZE 20
#define BOARD_MAX (BOARD_SIZE - 1)

#define PIECE_SIZE 5
#define PIECE_COUNT 21

struct Move {
    unsigned char color, piece, rotation, flipped, x, y;
};

#define COLOR_COUNT 4
#define ROTATION_COUNT 4
#define FLIPPED_COUNT 2

#define PRINT_BOARD(gameState) std::cout << std::endl; for (unsigned char y = 0; y < BOARD_SIZE; ++y) { for (unsigned char x = 0; x < BOARD_SIZE; ++x) { std::cout << ((gameState).board[0][x] & 1 << y ? "#" : "·")  << " "; } std::cout << std::endl; } std::cout << std::endl

#endif
