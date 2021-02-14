#include <cstdlib>
#include <algorithm>
#include "Evaluation.h"
#include "Pieces.h"

Evaluation::Evaluation() {
    for (int piece = 0; piece < PIECE_COUNT; ++piece) {
        for (int x = 0; x < PIECE_SIZE; ++x) {
            if (PIECES[piece][0][0][x] == 0) break;
            pieceEvaluation[piece] += __builtin_popcount(PIECES[piece][0][0][x]);
        }
    }

    for (U8 x = 0; x < BOARD_SIZE; ++x) {
        for (U8 y = 0; y < BOARD_SIZE; ++y) {
            coordsSortedByDistanceToMiddle.push_back({x, y, -(abs(BOARD_SIZE / 2 - x) + abs(BOARD_SIZE / 2 - y))});
        }
    }
    std::sort(coordsSortedByDistanceToMiddle.begin(), coordsSortedByDistanceToMiddle.end(),
              [](CoordValuePair a, CoordValuePair b) { return a.value > b.value; });
}

int Evaluation::evaluateDistanceToMiddle(GameState gameState, int color) {
    for (CoordValuePair coordinateValuePair : coordsSortedByDistanceToMiddle) {
        if (gameState.board[color + 1][coordinateValuePair.x] & 1 << coordinateValuePair.y) {
            return coordinateValuePair.value;
        }
    }
    return -BOARD_SIZE;
}

int evaluateValidFields(GameState gameState, int color) {
    int value = 0;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        U32 fields = ~(gameState.board[0][x] | gameState.horizontalNeighbours[color][x] |
                       gameState.verticalNeighbours[color][x]);
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

int hasWon(GameState gameState, int color) {
    for (int piece = PIECE_COUNT - 1; piece >= 0; --piece) {
        if (!gameState.deployedPieces[color][piece]) {
            return false;
        }
    }
    return true;
}

int Evaluation::evaluate(GameState gameState) {
    int value = 0;

    for (int color = 0; color < COLOR_COUNT; ++color) {
        int colorValue;

        if (gameState.turn >= 6 * COLOR_COUNT) {
            colorValue = evaluateValidFields(gameState, color);
            if (colorValue == 0) {
                colorValue -= WIN_SCORE;
            } else if (hasWon(gameState, color)) {
                colorValue += WIN_SCORE + 15;
            }
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
