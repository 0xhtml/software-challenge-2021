#include <chrono>
#include <iostream>
#include <vector>
#include "Algorithm.h"
#include "GameState.h"
#include "Types.h"

int main() {
    auto gameState = GameState{};
    for (int i = 0; i < 20; ++i) {
        std::vector<Move> possibleMoves = gameState.getPossibleMoves([](U8 p) { return false; });
        gameState.performMove(possibleMoves[i * 2 % possibleMoves.size()]);
    }
    PRINT_BOARD(gameState);

    Algorithm algorithm = Algorithm{};

    auto start = std::chrono::steady_clock::now();
    algorithm.run(gameState);
    auto end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
    return 0;
}
