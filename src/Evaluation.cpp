#include <algorithm>
#include "Evaluation.h"
#include "Pieces.h"

Evaluation::Evaluation() {
    for (int piece = 0; piece < PIECE_COUNT; ++piece) {
        for (int x = 0; x < PIECE_SIZE; ++x) {
            if (PIECES[piece][0][0][x] == 0) break;
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

int Evaluation::evaluateDistanceToMiddle(const GameState &gameState, const int color) const {
    for (PositionValuePair positionValuePair : positionsSortedByDistanceToMiddle) {
        if (gameState.board[color + 1][positionValuePair.x] & 1 << positionValuePair.y) {
            return positionValuePair.value;
        }
    }
    return -BOARD_SIZE;
}

void Evaluation::evaluatePieceSize(const GameState &gameState, const int color, int &value) const {
    for (int piece = 0; piece < PIECE_COUNT; ++piece) {
        if (gameState.deployedPieces[color][piece]) {
            value += pieceSize[piece];
        }
    }
}

void evaluateSpace(const GameState &gameState, const int team, int &value) {
    int corners[2][2], i = 0, side;

    for (int x : {0, BOARD_MAX}) {
        for (int y : {0, BOARD_MAX}) {
            if ((gameState.board[team + 1][x] | gameState.board[team + 3][x]) & 1 << y) {
                corners[i % 2][0] = x;
                corners[i % 2][1] = y;
                i++;
            }
        }
    }

    if (i < 2) {
        return;
    }

    if (corners[0][0] == 0 && corners[0][1] == 0) {
        if (corners[1][0] == 0) {
            side = 0;
        } else if (corners[1][1] == 0) {
            side = 1;
        } else {
            return;
        }
    } else if (corners[1][0] == BOARD_MAX && corners[1][1] == BOARD_MAX) {
        if (corners[0][0] == BOARD_MAX) {
            side = 2;
        } else if (corners[0][1] == BOARD_MAX) {
            side = 3;
        } else {
            return;
        }
    } else {
        return;
    }

    for (int x = side == 1 ? BOARD_SIZE / 2 : 0; x < (side == 0 ? BOARD_SIZE / 2 : BOARD_SIZE); ++x) {
        U32 mask = 0b11111111111111111111;
        switch (side) {
            case 0:
                mask = mask << x & mask >> x;
                break;
            case 1:
                mask >>= x < BOARD_SIZE / 2 ? BOARD_MAX - x : x;
                break;
            case 2:
                mask = mask << (BOARD_MAX - x) & mask >> (BOARD_MAX - x);
                break;
            case 3:
                mask <<= x < BOARD_SIZE / 2 ? BOARD_MAX - x : x;
                break;
        }
        value -= __builtin_popcount(gameState.board[0][x] & mask);

        U32 validFields = 0;
        for (int color : {team + 1, (team + 3) % COLOR_COUNT}) {
            U32 colorValidFields = ~(gameState.board[0][x] | gameState.horizontalNeighbours[color][x] |
                                     gameState.verticalNeighbours[color][x]);
            U32 validFieldsMask = 0;
            if (x < BOARD_MAX) {
                validFieldsMask |= gameState.verticalNeighbours[color][x + 1];
            }
            if (x > 0) {
                validFieldsMask |= gameState.verticalNeighbours[color][x - 1];
            }
            validFields |= colorValidFields & validFieldsMask;
        }
        value -= __builtin_popcount(validFields & mask) * 7;
    }
}

int Evaluation::evaluate(const GameState &gameState) const {
    int value = 0;

    for (int team = 0; team < TEAM_COUNT; ++team) {
        int teamValue = 0;

        if (gameState.turn < 4 * COLOR_COUNT) {
            teamValue += evaluateDistanceToMiddle(gameState, team);
            teamValue += evaluateDistanceToMiddle(gameState, team + 2);
        } else {
            evaluateSpace(gameState, team, teamValue);
            evaluatePieceSize(gameState, team, teamValue);
            evaluatePieceSize(gameState, team + 2, teamValue);
        }

        if (gameState.turn % 2 == team) {
            value += teamValue;
        } else {
            value -= teamValue;
        }
    }

    return value;
}
