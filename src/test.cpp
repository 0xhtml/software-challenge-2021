#include <iostream>
#include <random>
#include <queue>
#include <thread>
#include "Algorithm.h"
#include "Pieces.h"

#define GAME_COUNT 100

void worker(std::queue<int> *runQueue, std::queue<std::pair<int, int>> *resultQueue) {
    std::random_device randomDevice;
    std::uniform_int_distribution<int> distribution(0, PIECE_COUNT - 1);

    while (!runQueue->empty()) {
        runQueue->pop();

        GameState gameState{};
        gameState.firstPiece = distribution(randomDevice);

        Algorithm algorithm{};

        for (int i = 0; i < COLOR_COUNT * PIECE_COUNT; ++i) {
            gameState.gameOver[gameState.turn % COLOR_COUNT] = false;
            if (gameState.turn % 2 == 0) {
                gameState.performMove(algorithm.iterativeDeepening(gameState));
            } else {
                std::vector<Move> possibleMoves = gameState.getPossibleMoves();
                if (possibleMoves.empty()) possibleMoves.push_back({5});
                std::uniform_int_distribution<int> distribution1{0, static_cast<int>(possibleMoves.size() - 1)};
                gameState.performMove(possibleMoves[distribution1(randomDevice)]);
            }
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

        resultQueue->push(result);
    }
}

int main() {
    std::queue<int> runQueue;
    std::queue<std::pair<int, int>> resultQueue;

    for (int i = 0; i < GAME_COUNT; ++i) {
        runQueue.push(i);
    }

    unsigned int threadCount = std::thread::hardware_concurrency();
    std::cout << std::to_string(threadCount) << " threads" << std::endl;

    std::thread threads[threadCount];

    for (std::thread &thread : threads) {
        thread = std::thread(worker, &runQueue, &resultQueue);
    }

    for (std::thread &thread : threads) {
        thread.join();
    }

    double avgA = 0, avgB = 0;

    while (!resultQueue.empty()) {
        avgA += resultQueue.front().first;
        avgB += resultQueue.front().second;
        resultQueue.pop();
    }

    avgA /= GAME_COUNT;
    avgB /= GAME_COUNT;

    std::cout << std::endl << std::to_string(avgA) << " : " << std::to_string(avgB) << std::endl;

    return 0;
}
