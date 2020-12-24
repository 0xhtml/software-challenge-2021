#include "Hash.h"
#include "GameState.h"
#include "Types.h"

U64 rand64() {
    static U64 next = 1;
    next = next * 1103515245 + 12345;
    return next;
}

Hash::Hash() {
    for (auto &a : table) {
        for (auto &b : a) {
            for (auto &c : b) {
                c = rand64();
            }
        }
    }
}

U64 Hash::hash(GameState gameState) {
    U64 hash = 0;
    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int color = 0; color < COLOR_COUNT; ++color) {
                if (gameState.board[color + 1][x] & 1 << y) {
                    hash ^= table[x][y][color];
                    break;
                }
            }
        }
    }
    return hash;
}
