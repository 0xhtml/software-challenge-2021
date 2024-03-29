#include "GameState.h"

#include <vector>

#include "Types.h"

U32 GameState::getValidFields(const int color, const int x) const {
    U32 corners = horizontalNeighbours[color][x] << 1 | horizontalNeighbours[color][x] >> 1;
    corners &= ~(boardOR[x] | horizontalNeighbours[color][x] | verticalNeighbours[color][x]);
    return corners;
}

std::vector<Move> GameState::getPossibleMoves() const {
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
                if (boardOR[x] & 1 << y) {
                    continue;
                }

                if (turn <= 2) {
                    int dx = x == BOARD_MAX ? 0 : BOARD_MAX;
                    int dy = y == BOARD_MAX ? 0 : BOARD_MAX;

                    if (turn == 1 ? ~boardOR[dx] & 1 << dy : board[(turn + 2) % COLOR_COUNT][dx] & 1 << dy) {
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
    } else {
        for (U8 x = 0; x < BOARD_SIZE; ++x) {
            U32 corners = getValidFields(color, x);

            for (U8 y = 0; y < BOARD_SIZE; ++y) {
                if (!(corners & 1 << y)) continue;

                for (U8 piece = 0; piece < PIECE_COUNT; ++piece) {
                    if (deployedPieces[color][piece]) continue;

                    for (U8 rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
                        U8 pieceBoundsX = pieces.bounds[piece][rotation % 2 ? 1 : 0];
                        U8 pieceBoundsY = pieces.bounds[piece][rotation % 2 ? 0 : 1];

                        for (U8 flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                            if (PIECES[piece][rotation][flipped][0] == 0) break;

                            for (int corner = 0; corner < pieces.cornerCount[piece]; ++corner) {
                                U8 pieceX = pieces.corners[piece][rotation][flipped][corner][0];
                                if (pieceX > x) continue;

                                U8 pieceY = pieces.corners[piece][rotation][flipped][corner][1];
                                if (pieceY > y) continue;

                                pieceX = x - pieceX;
                                if (pieceX > BOARD_MAX - pieceBoundsX) continue;

                                pieceY = y - pieceY;
                                if (pieceY > BOARD_MAX - pieceBoundsY) continue;

                                bool valid = true;

                                for (int i = 0; i <= pieceBoundsX; ++i) {
                                    U32 shiftedPiece = PIECES[piece][rotation][flipped][i] << pieceY;

                                    if (shiftedPiece & boardOR[pieceX + i] ||
                                        shiftedPiece & horizontalNeighbours[color][pieceX + i] ||
                                        shiftedPiece & verticalNeighbours[color][pieceX + i]) {
                                        valid = false;
                                        break;
                                    }
                                }

                                if (valid) {
                                    possibleMoves.push_back({color, piece, rotation, flipped, pieceX, pieceY});
                                }
                            }
                        }
                    }
                }
            }
        }

        if (possibleMoves.empty()) possibleMoves.push_back({5});
    }

    return possibleMoves;
}

void GameState::performMove(const Move &move) {
    if (move.color < COLOR_COUNT) {
        for (int i = 0; i <= pieces.bounds[move.piece][move.rotation % 2 ? 1 : 0]; ++i) {
            U32 shiftedPiece = PIECES[move.piece][move.rotation][move.flipped][i] << move.y;
            int x = move.x + i;

            boardOR[x] |= shiftedPiece;
            board[move.color][x] |= shiftedPiece;

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

            boardOR[x] &= ~shiftedPiece;
            board[move.color][x] &= ~shiftedPiece;

            verticalNeighbours[move.color][x] = board[move.color][x] << 1 | board[move.color][x] >> 1;
            if (x > 0) {
                horizontalNeighbours[move.color][x - 1] = board[move.color][x];
                if (x > 1) horizontalNeighbours[move.color][x - 1] |= board[move.color][x - 2];
            }
        }

        if (x > 0 && x < BOARD_MAX) {
            horizontalNeighbours[move.color][x] = board[move.color][x - 1] | board[move.color][x + 1];
        } else if (x > 0) {
            horizontalNeighbours[move.color][x] = board[move.color][x - 1];
        } else {
            horizontalNeighbours[move.color][x] = board[move.color][x + 1];
        }

        if (x < BOARD_MAX - 1) {
            horizontalNeighbours[move.color][x + 1] = board[move.color][x] | board[move.color][x + 2];
        } else if (x < BOARD_MAX) {
            horizontalNeighbours[move.color][x + 1] = board[move.color][x];
        }

        boardHash ^= hash.hash(move);
        deployedPieces[move.color][move.piece] = false;
    }
    turn--;
}
