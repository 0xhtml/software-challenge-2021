#include "Hash.h"

unsigned long int rand64() {
    static unsigned long int next = 1;
    next = next * 1103515245 + 12345;
    return next;
}

Hash::Hash() {
    for (auto &a : table) {
        for (auto &b : a) {
            for (unsigned long &c : b) {
                c = rand64();
            }
        }
    }
}

unsigned long int Hash::hash(GameState gameState) {
    unsigned long int hash = 0;
    for (unsigned char x = 0; x < BOARD_SIZE; ++x) {
        for (unsigned char y = 0; y < BOARD_SIZE; ++y) {
            for (unsigned char color = 0; color < COLOR_COUNT; ++color) {
                if (gameState.board[color + 1][x] & 1 << y) {
                    hash ^= table[x][y][color];
                    break;
                }
            }
        }
    }
    return hash;
}
