#include <chrono>
#include <iostream>
#include "Algorithm.h"

int main() {
    auto *gameState = new GameState();
    for (int i = 0; i < 7; ++i) {
        std::vector<Move> *possibleMoves = gameState->getPossibleMoves();
        if (!possibleMoves->empty()) {
            gameState->performMove((*possibleMoves)[0]);
        }
    }
    PRINT_BOARD(gameState);
//    benchmarkGetPossibleMoves(gameState);
    auto start = std::chrono::steady_clock::now();
    (new Algorithm())->run(gameState);
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
    return 0;
}