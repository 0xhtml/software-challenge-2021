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
            if (value == (gameState.getTurn() % 4) + 1 || value == ((gameState.getTurn() + 2) % 4) + 1) {
                evaluation += 1;
            } else {
                evaluation -= 1;
            }
        }
    }

    for (auto &cacheEntry : cache) {
        if (cacheEntry.evaluation == 0) {
            cacheEntry.hash = gameStateHash;
            cacheEntry.evaluation = evaluation;
            break;
        }
    }

    return evaluation;
}