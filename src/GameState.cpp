#include "GameState.h"
#include "Pieces.h"

std::vector<Move> GameState::getPossibleMoves(bool(*filter)(unsigned char)) {
    auto possibleMoves = std::vector<Move>{};
    Move move{static_cast<unsigned char>(turn % COLOR_COUNT)};

    if (turn < 4) {
        move.piece = firstPiece;

        unsigned char xo = 0;
        unsigned char yo = 0;

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
        for (; move.piece < PIECE_COUNT; ++move.piece) {
            if (filter(move.piece) || deployedPieces[move.color][move.piece]) continue;

            for (move.rotation = 0; move.rotation < ROTATION_COUNT; ++move.rotation) {
                for (move.flipped = 0; move.flipped < FLIPPED_COUNT; ++move.flipped) {
                    if (PIECE(move)[0] == 0) continue;

                    for (move.x = 0; move.x < BOARD_SIZE - PIECE_BOUND_X(move); ++move.x) {
                        for (move.y = 0; move.y < BOARD_SIZE - PIECE_BOUND_Y(move); ++move.y) {
                            bool valid = true;
                            bool diagonal = false;

                            for (unsigned char x = 0; x < PIECE_SIZE; ++x) {
                                if (PIECE(move)[x] == 0) break;

                                unsigned int shiftedPiece = PIECE(move)[x] << move.y;

                                if (board[0][move.x + x] & shiftedPiece ||
                                    (move.x + x < BOARD_MAX && board[move.color + 1][move.x + x + 1] & shiftedPiece) ||
                                    (move.x + x > 0 && board[move.color + 1][move.x + x - 1] & shiftedPiece) ||
                                    (board[move.color + 1][move.x + x] & shiftedPiece << 1) ||
                                    (board[move.color + 1][move.x + x] & shiftedPiece >> 1)) {
                                    valid = false;
                                    break;
                                }

                                if (!diagonal && (move.x + x < BOARD_MAX && board[move.color + 1][move.x + x + 1] &
                                                                            (shiftedPiece << 1 | shiftedPiece >> 1)) ||
                                    (move.x + x > 0 &&
                                     board[move.color + 1][move.x + x - 1] & (shiftedPiece << 1 | shiftedPiece >> 1))) {
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
        move.piece = 250;
        possibleMoves.push_back(move);
    }
    return possibleMoves;
}

void GameState::performMove(Move move) {
    if (move.piece < PIECE_COUNT) {
        for (unsigned char i = 0; i < PIECE_SIZE; ++i) {
            board[0][move.x + i] |= PIECE(move)[i] << move.y;
            board[move.color + 1][move.x + i] |= PIECE(move)[i] << move.y;
        }
        deployedPieces[move.color][move.piece] = 1;
    }
    turn++;
}

void GameState::undoMove(Move move) {
    if (move.piece < PIECE_COUNT) {
        for (unsigned char i = 0; i < PIECE_SIZE; ++i) {
            board[0][move.x + i] ^= PIECE(move)[i] << move.y;
            board[move.color + 1][move.x + i] ^= PIECE(move)[i] << move.y;
        }
        deployedPieces[move.color][move.piece] = 0;
    }
    turn--;
}
