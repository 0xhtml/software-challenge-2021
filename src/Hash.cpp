#include <cstdlib>
#include "Hash.h"

unsigned long int rand64() {
    return (unsigned long int) rand() << (unsigned) 0 & 0x000000000000FFFFull |
           (unsigned long int) rand() << (unsigned) 16 & 0x00000000FFFF0000ull |
           (unsigned long int) rand() << (unsigned) 32 & 0x0000FFFF00000000ull |
           (unsigned long int) rand() << (unsigned) 48 & 0xFFFF000000000000ull;
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
            unsigned char value = gameState.boardGet(x, y);
            if (value) hash ^= table[x][y][value - 1];
        }
    }
    return hash;
}
