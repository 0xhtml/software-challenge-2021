#include "GameState.h"
#include "Evaluation.h"
#include "Pieces.h"

std::vector<Move> GameState::getPossibleMoves() {
    auto possibleMoves = std::vector<Move>{};
    Move move{static_cast<U8>(turn % COLOR_COUNT)};

    if (turn < 4) {
        move.piece = firstPiece;

        U8 xo = 0;
        U8 yo = 0;

        for (; move.rotation < ROTATION_COUNT; ++move.rotation) {
            if (turn == 1 || turn == 2) {
                xo = PIECE_BOUND_X(move);
                move.x = BOARD_MAX - xo;
            }
            if (turn > 1) {
                yo = PIECE_BOUND_Y(move);
                move.y = BOARD_MAX - yo;
            }
            for (move.flipped = 0; move.flipped < FLIPPED_COUNT; ++move.flipped) {
                if (PIECE(move)[xo] & 1 << yo) possibleMoves.push_back(move);
            }
        }
    } else {
        U32 horizontalNeighbours[BOARD_SIZE]{};
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (x > 0) {
                horizontalNeighbours[x] |= board[move.color + 1][x - 1];
            }
            if (x < BOARD_MAX) {
                horizontalNeighbours[x] |= board[move.color + 1][x + 1];
            }
        }

        for (; move.piece < PIECE_COUNT; ++move.piece) {
            if (deployedPieces[move.color][move.piece]) continue;

            for (move.rotation = 0; move.rotation < ROTATION_COUNT; ++move.rotation) {
                for (move.flipped = 0; move.flipped < FLIPPED_COUNT; ++move.flipped) {
                    if (PIECE(move)[0] == 0) continue;

                    for (move.x = 0; move.x < BOARD_SIZE - PIECE_BOUND_X(move); ++move.x) {
                        for (move.y = 0; move.y < BOARD_SIZE - PIECE_BOUND_Y(move); ++move.y) {
                            bool valid = true;
                            bool diagonal = false;

                            for (int x = 0; x < PIECE_SIZE; ++x) {
                                if (PIECE(move)[x] == 0) break;

                                U32 shiftedPiece = PIECE(move)[x] << move.y;

                                if (shiftedPiece & board[0][move.x + x] ||
                                    shiftedPiece & horizontalNeighbours[move.x + x] ||
                                    shiftedPiece & verticalNeighbours[move.color][move.x + x]) {
                                    valid = false;
                                    break;
                                }

                                if (!diagonal && ((move.x + x < BOARD_MAX &&
                                                   shiftedPiece & verticalNeighbours[move.color][move.x + x + 1]) ||
                                                  (move.x + x > 0 &&
                                                   shiftedPiece & verticalNeighbours[move.color][move.x + x - 1]))) {
                                    diagonal = true;
                                }
                            }

                            if (valid && diagonal) {
                                possibleMoves.push_back(move);
                            }
                        }
                    }
                }
            }
        }
    }

    if (possibleMoves.empty()) {
        possibleMoves.push_back({5});
    }
    return possibleMoves;
}

void GameState::performMove(Move move) {
    if (move.color < COLOR_COUNT) {
        for (int i = 0; i < PIECE_SIZE; ++i) {
            U32 shiftedPiece = PIECE(move)[i] << move.y;
            board[0][move.x + i] |= shiftedPiece;
            board[move.color + 1][move.x + i] |= shiftedPiece;
            verticalNeighbours[move.color][move.x + i] |= shiftedPiece << 1 | shiftedPiece >> 1;
        }
        gameStateHash ^= hash.hash(move);
        deployedPieces[move.color][move.piece] = true;
        evaluation += Evaluation::evaluate(move) * (move.color % 2 ? -1 : 1);
    }
    turn++;
}

void GameState::undoMove(Move move) {
    if (move.color < COLOR_COUNT) {
        for (int i = 0; i < PIECE_SIZE; ++i) {
            U32 shiftedPiece = PIECE(move)[i] << move.y;
            board[0][move.x + i] ^= shiftedPiece;
            board[move.color + 1][move.x + i] ^= shiftedPiece;
            verticalNeighbours[move.color][move.x + i] =
                    board[move.color + 1][move.x + i] << 1 | board[move.color + 1][move.x + i] >> 1;
        }
        gameStateHash ^= hash.hash(move);
        deployedPieces[move.color][move.piece] = false;
        evaluation -= Evaluation::evaluate(move) * (move.color % 2 ? -1 : 1);
    }
    turn--;
}
