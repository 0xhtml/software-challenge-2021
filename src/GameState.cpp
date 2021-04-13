#include "GameState.h"
#include "Pieces.h"

std::vector<Move> GameState::getPossibleMoves() {
    std::vector<Move> possibleMoves;
    U8 color = turn % COLOR_COUNT;

    if (turn == 0) {
        for (U8 rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
            for (U8 flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                if (PIECES[firstPiece][rotation][flipped][0] & 1) {
                    possibleMoves.push_back({color, firstPiece, rotation, flipped, 0, 0});
                }
            }
        }
    } else if (turn < COLOR_COUNT) {
        for (U8 x : {0, BOARD_MAX}) {
            for (U8 y : {0, BOARD_MAX}) {
                if (board[0][x] & 1 << y) {
                    continue;
                }

                if (turn <= 2) {
                    int dx = x == BOARD_MAX ? 0 : BOARD_MAX;
                    int dy = y == BOARD_MAX ? 0 : BOARD_MAX;

                    if (turn == 1 ? ~board[0][dx] & 1 << dy : board[(turn + 2) % COLOR_COUNT + 1][dx] & 1 << dy) {
                        continue;
                    }
                }

                for (U8 rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
                    U8 xo = 0;
                    U8 yo = 0;

                    if (x) {
                        xo = pieces.bounds[firstPiece][rotation % 2 ? 1 : 0];
                    }
                    if (y) {
                        yo = pieces.bounds[firstPiece][rotation % 2 ? 0 : 1];
                    }

                    U8 moveX = x - xo;
                    U8 moveY = y - yo;

                    for (U8 flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                        if (PIECES[firstPiece][rotation][flipped][xo] & 1 << yo) {
                            possibleMoves.push_back({color, firstPiece, rotation, flipped, moveX, moveY});
                        }
                    }
                }

                return possibleMoves;
            }
        }
    } else if (!gameOver[color]) {
        for (U8 piece = 0; piece < PIECE_COUNT; ++piece) {
            if (deployedPieces[color][piece]) continue;

            for (U8 rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
                for (U8 flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                    if (PIECES[piece][rotation][flipped][0] == 0) continue;

                    U8 pieceBoundsX = pieces.bounds[piece][rotation % 2 ? 1 : 0];
                    U8 pieceBoundsY = pieces.bounds[piece][rotation % 2 ? 0 : 1];

                    for (U8 x = 0; x < BOARD_SIZE - pieceBoundsX; ++x) {
                        for (U8 y = 0; y < BOARD_SIZE - pieceBoundsY; ++y) {
                            bool valid = true;
                            bool diagonal = false;

                            for (int i = 0; i <= pieceBoundsX; ++i) {
                                U32 shiftedPiece = PIECES[piece][rotation][flipped][i] << y;

                                if (shiftedPiece & board[0][x + i] ||
                                    shiftedPiece & horizontalNeighbours[color][x + i] ||
                                    shiftedPiece & verticalNeighbours[color][x + i]) {
                                    valid = false;
                                    break;
                                }

                                if (!diagonal && (shiftedPiece << 1 & horizontalNeighbours[color][x + i] ||
                                                  shiftedPiece >> 1 & horizontalNeighbours[color][x + i])) {
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

        if (possibleMoves.empty()) {
            possibleMoves.push_back({5});
            gameOver[color] = true;
        }
    } else {
        possibleMoves.push_back({5});
    }

    return possibleMoves;
}

void GameState::performMove(const Move &move) {
    if (move.color < COLOR_COUNT) {
        for (int i = 0; i <= pieces.bounds[move.piece][move.rotation % 2 ? 1 : 0]; ++i) {
            U32 shiftedPiece = PIECES[move.piece][move.rotation][move.flipped][i] << move.y;
            int x = move.x + i;

            board[0][x] |= shiftedPiece;
            board[move.color + 1][x] |= shiftedPiece;

            verticalNeighbours[move.color][x] |= shiftedPiece << 1 | shiftedPiece >> 1;
            if (x > 0) horizontalNeighbours[move.color][x - 1] |= shiftedPiece;
            if (x < BOARD_MAX) horizontalNeighbours[move.color][x + 1] |= shiftedPiece;
        }

        boardHash ^= hash.hash(move);
        deployedPieces[move.color][move.piece] = true;
    }
    turn++;
}

void GameState::undoMove(const Move &move) {
    if (move.color < COLOR_COUNT) {
        int x;
        for (int i = 0; i <= pieces.bounds[move.piece][move.rotation % 2 ? 1 : 0]; ++i) {
            U32 shiftedPiece = PIECES[move.piece][move.rotation][move.flipped][i] << move.y;
            x = move.x + i;

            board[0][x] &= ~shiftedPiece;
            board[move.color + 1][x] &= ~shiftedPiece;

            verticalNeighbours[move.color][x] = board[move.color + 1][x] << 1 | board[move.color + 1][x] >> 1;
            if (x > 0) {
                horizontalNeighbours[move.color][x - 1] = board[move.color + 1][x];
                if (x > 1) horizontalNeighbours[move.color][x - 1] |= board[move.color + 1][x - 2];
            }
        }

        if (x > 0 && x < BOARD_MAX) {
            horizontalNeighbours[move.color][x] = board[move.color + 1][x - 1] | board[move.color + 1][x + 1];
        } else if (x > 0) {
            horizontalNeighbours[move.color][x] = board[move.color + 1][x - 1];
        } else {
            horizontalNeighbours[move.color][x] = board[move.color + 1][x + 1];
        }

        if (x < BOARD_MAX - 1) {
            horizontalNeighbours[move.color][x + 1] = board[move.color + 1][x] | board[move.color + 1][x + 2];
        } else if (x < BOARD_MAX) {
            horizontalNeighbours[move.color][x + 1] = board[move.color + 1][x];
        }

        boardHash ^= hash.hash(move);
        deployedPieces[move.color][move.piece] = false;
        gameOver[move.color] = false;
    }
    turn--;
}
