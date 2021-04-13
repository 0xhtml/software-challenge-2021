#include <random>
#include <iostream>
#include <benchmark/benchmark.h>
#include "../src/Algorithm.h"
#include "../src/GameState.h"

#define SETUP_RANDOM_DISTRIBUTIONS() \
    std::mt19937 mt19937(std::random_device{}()); \
    std::uniform_int_distribution<> firstPieceDistribution{0, 15}; \
    std::uniform_int_distribution<> turnDistribution{4, 40}

#define GAMELOOP \
    GameState gameState{}; \
    gameState.firstPiece = firstPieceDistribution(mt19937); \
    Algorithm algorithm{}; \
    const int turns = turnDistribution(mt19937); \
    while (gameState.turn < turns)

#define GAMELOOP_RANDOM_MOVE() \
    gameState.gameOver[gameState.turn % COLOR_COUNT] = false; \
    std::vector<Move> moves = gameState.getPossibleMoves(); \
    std::uniform_int_distribution<> disribution(0, moves.size() - 1); \
    gameState.performMove(moves[disribution(mt19937)])

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
