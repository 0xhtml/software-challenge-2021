#include "GameState.h"
#include "Evaluation.h"
#include "Pieces.h"

std::vector<Move> GameState::getPossibleMoves() {
    auto possibleMoves = std::vector<Move>{};
    U8 color = turn % COLOR_COUNT;

    if (turn < 4) {
        for (U8 rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
            U8 xo = 0;
            U8 yo = 0;
            U8 x = 0;
            U8 y = 0;

            if (turn == 1 || turn == 2) {
                xo = PIECE_BOUNDS[firstPiece][rotation % 2 ? 1 : 0];
                x = BOARD_MAX - xo;
            }
            if (turn > 1) {
                yo = PIECE_BOUNDS[firstPiece][rotation % 2 ? 0 : 1];
                y = BOARD_MAX - yo;
            }

            for (U8 flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                if (PIECES[firstPiece][rotation][flipped][xo] & 1 << yo)
                    possibleMoves.push_back({color, firstPiece, rotation, flipped, x, y});
            }
        }
    } else {
        U32 horizontalNeighbours[BOARD_SIZE]{};
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (x > 0) {
                horizontalNeighbours[x] |= board[color + 1][x - 1];
            }
            if (x < BOARD_MAX) {
                horizontalNeighbours[x] |= board[color + 1][x + 1];
            }
        }

        for (U8 piece = 0; piece < PIECE_COUNT; ++piece) {
            if (deployedPieces[color][piece]) continue;

            for (U8 rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
                for (U8 flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                    if (PIECES[piece][rotation][flipped][0] == 0) continue;

                    for (U8 x = 0; x < BOARD_SIZE - PIECE_BOUNDS[piece][rotation % 2 ? 1 : 0]; ++x) {
                        for (U8 y = 0; y < BOARD_SIZE - PIECE_BOUNDS[piece][rotation % 2 ? 0 : 1]; ++y) {
                            bool valid = true;
                            bool diagonal = false;

                            for (int i = 0; i < PIECE_SIZE; ++i) {
                                if (PIECES[piece][rotation][flipped][i] == 0) break;

                                U32 shiftedPiece = PIECES[piece][rotation][flipped][i] << y;

                                if (shiftedPiece & board[0][x + i] || shiftedPiece & horizontalNeighbours[x + i] ||
                                    shiftedPiece & verticalNeighbours[color][x + i]) {
                                    valid = false;
                                    break;
                                }

                                if (!diagonal &&
                                    ((x + i < BOARD_MAX && shiftedPiece & verticalNeighbours[color][x + i + 1]) ||
                                     (x + i > 0 && shiftedPiece & verticalNeighbours[color][x + i - 1]))) {
                                    diagonal = true;
                                }
                            }

                            if (valid && diagonal) {
                                possibleMoves.push_back({color, piece, rotation, flipped, x, y});
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
            U32 shiftedPiece = PIECES[move.piece][move.rotation][move.flipped][i] << move.y;
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
            U32 shiftedPiece = PIECES[move.piece][move.rotation][move.flipped][i] << move.y;
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
