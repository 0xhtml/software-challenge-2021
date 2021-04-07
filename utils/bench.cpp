#include <random>
#include <iostream>
#include <benchmark/benchmark.h>
#include "../src/Algorithm.h"
#include "../src/GameState.h"

#define SETUP_RANDOM_DISTRIBUTIONS() \
    std::random_device randomDevice; \
    std::uniform_int_distribution<int> firstPieceDistribution{0, 15}; \
    std::uniform_int_distribution<int> turnDistribution{4, 40}

#define GAMELOOP \
    GameState gameState{}; \
    gameState.firstPiece = firstPieceDistribution(randomDevice); \
    Algorithm algorithm{}; \
    const int turns = turnDistribution(randomDevice); \
    while (gameState.turn < turns)

#define GAMELOOP_RANDOM_MOVE() \
    gameState.gameOver[gameState.turn % COLOR_COUNT] = false; \
    std::vector<Move> moves = gameState.getPossibleMoves(); \
    gameState.performMove(moves[std::uniform_int_distribution<int>(0, moves.size() - 1)(randomDevice)])

#define GAMELOOP_ALGORITHM_MOVE() \
    gameState.gameOver[gameState.turn % COLOR_COUNT] = false; \
    gameState.performMove(algorithm.alphaBetaRoot(gameState, 1, -1000, 1000))

static void BM_GameState_getPossibleMoves(benchmark::State &state) {
    SETUP_RANDOM_DISTRIBUTIONS();

    for (auto _ : state) {
        state.PauseTiming();
        GAMELOOP {
            GAMELOOP_RANDOM_MOVE();
        }
        state.ResumeTiming();

        gameState.getPossibleMoves();
    }
}
BENCHMARK(BM_GameState_getPossibleMoves)->Unit(benchmark::kMillisecond);

static void BM_Algorithm_alphaBeta(benchmark::State &state) {
    SETUP_RANDOM_DISTRIBUTIONS();

    for (auto _ : state) {
        state.PauseTiming();
        GAMELOOP {
            GAMELOOP_ALGORITHM_MOVE();
            GAMELOOP_RANDOM_MOVE();
        }
        state.ResumeTiming();

        for (int depth = 1; depth <= 3; ++depth) {
            algorithm.alphaBetaRoot(gameState, depth, -1000, 1000);
        }
    }
}
BENCHMARK(BM_Algorithm_alphaBeta)->Threads(8)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
