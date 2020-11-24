#include "Evaluation.h"

int Evaluation::evaluate(GameState gameState) {
    unsigned long int gameStateHash = hash.hash(gameState);

    for (auto &cacheEntry : cache) {
        if (cacheEntry.hash == gameStateHash) {
            return cacheEntry.evaluation;
        }
    }

    int evaluation = 0;
    for (unsigned char x = 0; x < BOARD_SIZE; ++x) {
        for (unsigned char y = 0; y < BOARD_SIZE; ++y) {
            int value = gameState.boardGet(x, y);
            if (!value) continue;
            (value - 1) % 2 == gameState.getTurn() % 2 ? (evaluation++) : (evaluation--);
        }
    }

    for (int i = 0; i < 2; ++i) {
        for (auto &cacheEntry : cache) {
            if (cacheEntry.evaluation == 0 && (i > 0 || cacheEntry.hash == 0)) {
                cacheEntry.hash = gameStateHash;
                cacheEntry.evaluation = evaluation;
                return evaluation;
            }
        }
    }

    return evaluation;
}
