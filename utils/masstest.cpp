#include <iostream>
#include <random>
#include <queue>
#include <thread>
#include "../src/Algorithm.h"
#include "../src/Pieces.h"

#define GAME_COUNT 1000
#define THREADS 8

void worker(std::queue<bool> *counter, std::queue<std::pair<int, int>> *results) {
    std::mt19937 mt19937{std::random_device{}()};
    std::uniform_int_distribution<> firstPieceDistribution(0, 15);

    while (!counter->empty()) {
        counter->pop();

        GameState gameState{};
        gameState.firstPiece = firstPieceDistribution(mt19937);

        Algorithm algorithm{};

        for (int i = 0; i < TEAM_COUNT * PIECE_COUNT; ++i) {
            gameState.performMove(algorithm.iterativeDeepening(gameState));

            std::vector<Move> possibleMoves = gameState.getPossibleMoves();
            Move move = possibleMoves[std::uniform_int_distribution<>(0, possibleMoves.size() - 1)(mt19937)];
            gameState.performMove(move);
        }

        std::pair<int, int> result;

        for (int team = 0; team < TEAM_COUNT; ++team) {
            int points = 0;

            for (int color : {team, team + 2}) {
                bool allPieces = true;

                for (int piece = 0; piece < PIECE_COUNT; ++piece) {
                    for (int x = 0; x < PIECE_SIZE; ++x) {
                        if (gameState.deployedPieces[color][piece]) {
                            points += __builtin_popcount(PIECES[piece][0][0][x]);
                        } else {
                            allPieces = false;
                        }
                    }
                }

                if (allPieces) points += 15;
            }

            (team == 0 ? result.first : result.second) = points;
        }

        results->push(result);
    }
}

int main() {
    std::queue<bool> counter;
    std::queue<std::pair<int, int>> results;

    for (int i = 0; i < GAME_COUNT; ++i) {
        counter.push(true);
    }

    std::thread threads[THREADS];

    for (std::thread &thread : threads) {
        thread = std::thread(worker, &counter, &results);
    }

    for (std::thread &thread : threads) {
        thread.join();
    }

    double avgA = 0, avgB = 0;

    while (!results.empty()) {
        avgA += results.front().first;
        avgB += results.front().second;
        results.pop();
    }

    avgA /= GAME_COUNT;
    avgB /= GAME_COUNT;

    std::cout << std::endl << std::to_string(avgA) << " : " << std::to_string(avgB) << std::endl;

    return 0;
}
