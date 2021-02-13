#include <cstdlib>
#include "Evaluation.h"
#include "Pieces.h"

int evaluateDistanceToMiddle(GameState gameState, int color) {
    int minDistance = BOARD_SIZE;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if (gameState.board[color + 1][x] & 1 << y) {
                int distance = abs(BOARD_SIZE / 2 - x) + abs(BOARD_SIZE / 2 - y);

                if (minDistance > distance) {
                    minDistance = distance;
                }
            }
        }
    }

    return -minDistance;
}

int evaluateValidFields(GameState gameState, int color) {
    int value = 0;

    U32 horizontalNeighbours[BOARD_SIZE]{};
    for (int x = 0; x < BOARD_SIZE; ++x) {
        if (x > 0) {
            horizontalNeighbours[x] |= gameState.board[color + 1][x - 1];
        }
        if (x < BOARD_MAX) {
            horizontalNeighbours[x] |= gameState.board[color + 1][x + 1];
        }
    }

    for (int x = 0; x < BOARD_SIZE; ++x) {
        U32 fields = ~(gameState.board[0][x] | horizontalNeighbours[x] | gameState.verticalNeighbours[color][x]);
        U32 mask = 0;
        if (x < BOARD_MAX) {
            mask |= gameState.verticalNeighbours[color][x + 1];
        }
        if (x > 0) {
            mask |= gameState.verticalNeighbours[color][x - 1];
        }
        fields &= mask;
        value += __builtin_popcount(fields);
    }

    return value;
}

int evaluateWinner(GameState gameState, int color) {
    for (bool pieceDeployed : gameState.deployedPieces[color]) {
        if (!pieceDeployed) {
            return 0;
        }
    }
    return 1;
}

int Evaluation::evaluate(GameState gameState) {
    int value = 0;

    for (int color = 0; color < COLOR_COUNT; ++color) {
        int colorValue;

        if (gameState.turn >= 6 * COLOR_COUNT) {
            colorValue = evaluateValidFields(gameState, color);
            colorValue += evaluateWinner(gameState, color) * 100;
        } else {
            colorValue = evaluateDistanceToMiddle(gameState, color);
        }
        if (gameState.deployedPieces[color][0]) {
            colorValue -= 15;
        }

        if (color % 2 == gameState.turn % 2) {
            value += colorValue;
        } else {
            value -= colorValue;
        }
    }

    return value;
}
