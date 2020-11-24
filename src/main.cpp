#include <chrono>
#include <iostream>
#include "Algorithm.h"

int main() {
    auto gameState = GameState{};
    for (int i = 0; i < 20; ++i) {
        std::vector<Move> possibleMoves = gameState.getPossibleMoves([](unsigned char p) { return false; });
        if (!possibleMoves.empty()) {
            gameState.performMove(possibleMoves[0]);
        }
    }
    PRINT_BOARD(gameState);

    Algorithm algorithm = Algorithm{};

    auto start = std::chrono::steady_clock::now();
    algorithm.run(gameState);
    auto end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
    return 0;
}
