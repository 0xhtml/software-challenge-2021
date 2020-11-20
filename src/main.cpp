#include <vector>
#include <chrono>
#include <iostream>
#include "GameState.h"

void benchmarkGetPossibleMoves(GameState *gameState) {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000; ++i) {
        gameState->getPossibleMoves();
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms  ± 20ms";
}

int main() {
    auto *gameState = new GameState();
    for (int i = 0; i < 5; ++i) {
        std::vector<Move> *possibleMoves = gameState->getPossibleMoves();
        gameState->performMove((*possibleMoves)[0]);
    }
    PRINT_BOARD(gameState);
    benchmarkGetPossibleMoves(gameState);
    return 0;
}