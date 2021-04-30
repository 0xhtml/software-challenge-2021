#include "Evaluation.h"

#include <algorithm>

#include "Constants.h"
#include "GameState.h"
#include "Pieces.h"

Evaluation::Evaluation() {
    for (int piece = 0; piece < PIECE_COUNT; ++piece) {
        for (int x = 0; x < PIECE_SIZE; ++x) {
            pieceSize[piece] += __builtin_popcount(PIECES[piece][0][0][x]);
        }
    }

    for (U8 x = 0; x < BOARD_SIZE; ++x) {
        for (U8 y = 0; y < BOARD_SIZE; ++y) {
            positionsSortedByDistanceToMiddle.push_back({x, y, -(abs(BOARD_SIZE / 2 - x) + abs(BOARD_SIZE / 2 - y))});
        }
    }
    std::sort(positionsSortedByDistanceToMiddle.begin(), positionsSortedByDistanceToMiddle.end(),
              [](PositionValuePair a, PositionValuePair b) { return a.value > b.value; });
}

int Evaluation::distanceToMiddle(const GameState &gameState, const int color) const {
    for (PositionValuePair positionValuePair : positionsSortedByDistanceToMiddle) {
        if (gameState.board[color][positionValuePair.x] & 1 << positionValuePair.y) {
            return positionValuePair.value;
        }
    }
    return -BOARD_SIZE;
}

int Evaluation::getSide(const GameState &gameState, const int team) const {
    U32 first = gameState.board[team][0] | gameState.board[team + 2][0];
    U32 last = gameState.board[team][BOARD_MAX] | gameState.board[team + 2][BOARD_MAX];

    if (first & 1) { // x=0, y=0
        if (first & 1 << BOARD_MAX) return 0; // x=0, y=19
        if (last & 1) return 1; // x=19,y=0
    }

    if (last & 1 << BOARD_MAX) { // x=19, y=19
        if (last & 1) return 2; // x=19,y=0
        if (first & 1 << BOARD_MAX) return 3; // x=0,y=19
    }

    return -1;
}

void Evaluation::evaluateSpace(const GameState &gameState, const int team, int &value) const {
    int side = getSide(gameState, team);
    if (side == -1) return;

    for (int x = side == 1 ? BOARD_SIZE / 2 : 0; x < (side == 0 ? BOARD_SIZE / 2 : BOARD_SIZE); ++x) {
        U32 mask;

        switch (side) {
            case 0:
                mask = 0xFFFFF << x & 0xFFFFF >> x;
                break;
            case 1:
                mask = 0xFFFFF >> (x < BOARD_MAX / 2 ? BOARD_MAX - x : x);
                break;
            case 2:
                mask = 0xFFFFF << (BOARD_MAX - x) & 0xFFFFF >> (BOARD_MAX - x);
                break;
            case 3:
                mask = 0xFFFFF << (x < BOARD_MAX / 2 ? BOARD_MAX - x : x);
        }

        value -= __builtin_popcount(gameState.boardOR[x] & mask);
        value -= __builtin_popcount(gameState.board[team + 1][x] & mask);
        value -= __builtin_popcount(gameState.board[(team + 3) % COLOR_COUNT][x] & mask);

        U32 validFields = gameState.getValidFields(team + 1, x);
        validFields |= gameState.getValidFields((team + 3) % COLOR_COUNT, x);
        value -= __builtin_popcount(validFields & mask) * 5;
    }
}

int Evaluation::evaluate(const GameState &gameState) const {
    int value = 0;

    for (int color = 0; color < COLOR_COUNT; ++color) {
        int colorValue = 0;

        if (gameState.turn < 15) {
            colorValue += distanceToMiddle(gameState, color);

            if (gameState.deployedPieces[color][12]) ++colorValue;
            if (gameState.deployedPieces[color][14]) ++colorValue;
            if (gameState.deployedPieces[color][15]) ++colorValue;
        } else {
            if (color < TEAM_COUNT) {
                evaluateSpace(gameState, color, colorValue);
            }

            for (int piece = 0; piece < PIECE_COUNT; ++piece) {
                if (gameState.deployedPieces[color][piece]) {
                    colorValue += pieceSize[piece] * 4;
                }
            }

            if (gameState.deployedPieces[color][14]) ++colorValue;
            if (gameState.deployedPieces[color][15]) ++colorValue;
            if (gameState.deployedPieces[color][16]) ++colorValue;
            if (gameState.deployedPieces[color][17]) ++colorValue;
        }

        if (gameState.turn % 2 == color % 2) {
            value += colorValue;
        } else {
            value -= colorValue;
        }
    }

    return value;
}
