#include <cstdlib>
#include "Hash.h"

unsigned long int rand64() {
    return (unsigned long int) rand() & 0x00000000FFFFFFFFull | // NOLINT(cert-msc50-cpp)
           (unsigned long int) rand() << (unsigned) 32 & 0xFFFFFFFF00000000ull; // NOLINT(cert-msc50-cpp)
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
