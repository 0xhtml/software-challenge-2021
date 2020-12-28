#include "Hash.h"
#include "Pieces.h"

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

U64 Hash::hash(Move move) {
    U64 hash = 0;
    for (int x = 0; x < PIECE_SIZE; ++x) {
        for (int y = 0; y < PIECE_SIZE; ++y) {
            if (PIECE(move)[x] & 1 << y) {
                hash ^= table[move.x + x][move.y + y][move.color];
            }
        }
    }
    return hash;
}
