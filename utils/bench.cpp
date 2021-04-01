#include <random>
#include <benchmark/benchmark.h>
#include "../src/GameState.h"

static void BM_GameState_getPossibleMoves(benchmark::State &state) {
    std::random_device randomDevice;
    std::uniform_int_distribution<int> firstPieceDistribution{0, 15};
    std::uniform_int_distribution<int> turnDistribution{4, 40};

    for (auto _ : state) {
        state.PauseTiming();

        GameState gameState{};
        gameState.firstPiece = firstPieceDistribution(randomDevice);

        int turns = turnDistribution(randomDevice);
        while (gameState.turn < turns) {
            gameState.gameOver[gameState.turn % COLOR_COUNT] = false;

            std::vector<Move> possibleMoves = gameState.getPossibleMoves();
            if (possibleMoves.empty()) possibleMoves.push_back({5});

            Move move = possibleMoves[std::uniform_int_distribution<int>(0, possibleMoves.size() - 1)(randomDevice)];

            gameState.performMove(move);
        }

        state.ResumeTiming();

        gameState.getPossibleMoves();
    }
}

BENCHMARK(BM_GameState_getPossibleMoves)->Unit(benchmark::kMillisecond);
BENCHMARK_MAIN();
