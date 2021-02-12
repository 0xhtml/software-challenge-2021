#include "Hash.h"
#include "Pieces.h"

Hash::Hash() {
    U64 next = 1;
    for (auto &a : table) {
        for (auto &b : a) {
            for (auto &c : b) {
                next = next * 1103515245 + 12345;
                c = next;
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
