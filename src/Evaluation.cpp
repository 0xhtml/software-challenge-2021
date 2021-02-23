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

void Evaluation::evaluateFields(GameState gameState, int color, int &value) {
    for (int piece = 0; piece < PIECE_SIZE; ++piece) {
        if (gameState.deployedPieces[color][piece]) {
            value += pieceEvaluation[piece];
        }
    }
}

bool hasWon(GameState gameState, int color) {
    for (int piece = 0; piece < PIECE_COUNT; ++piece) {
        if (!gameState.deployedPieces[color][piece]) {
            return false;
        }
    }
    return true;
}

void evaluateSpace(GameState gameState, int color, int &value) {
    int startX = -1, startY, endX, endY;

    for (int x : {0, BOARD_MAX}) {
        for (int y : {0, BOARD_MAX}) {
            if (gameState.board[color + 1][x] & 1 << y) {
                startX = x;
                startY = y;

                if (x) {
                    endX = BOARD_SIZE / 2 - 1;
                } else {
                    endX = BOARD_SIZE / 2 + 1;
                }

                if (y) {
                    endY = BOARD_SIZE / 2 - 1;
                } else {
                    endY = BOARD_SIZE / 2 + 1;
                }
            }
        }
    }

    if (startX == -1) {
        return;
    }

    for (int x = startX; x != endX; startX ? --x : ++x) {
        for (int y = startY; y != endY; startY ? --y : ++y) {
            if (gameState.board[0][x] & 1 << y) {
                value -= 1;
            }
        }
    }
}

int Evaluation::evaluate(GameState gameState) {
    int value = 0;

    for (int color = 0; color < COLOR_COUNT; ++color) {
        int colorValue = 0;

        if (gameState.turn >= 4 * COLOR_COUNT) {
            evaluateFields(gameState, color, colorValue);
            evaluateSpace(gameState, color, colorValue);

            if (hasWon(gameState, color)) {
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
