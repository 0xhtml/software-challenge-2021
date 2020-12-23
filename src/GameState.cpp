#include "GameState.h"

GameState::GameState() {
    for (unsigned char piece = 0; piece < PIECE_COUNT; ++piece) {
        for (unsigned char rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
            for (unsigned char flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                pieces[piece][rotation][flipped].coord_count = PIECES[piece].coord_count;
                pieces[piece][rotation][flipped].diagonal_coord_count = PIECES[piece].diagonal_coord_count;
                unsigned char minX = PIECE_COORD_COUNT;
                unsigned char minY = PIECE_COORD_COUNT;
                unsigned char maxX = 0;
                unsigned char maxY = 0;
                for (unsigned char i = 0; i < PIECES[piece].coord_count; ++i) {
                    unsigned char x, y;
                    switch (rotation) {
                        case 1:
                            x = PIECE_COORD_MAX - PIECES[piece].coords[i][1];
                            y = PIECES[piece].coords[i][0];
                            break;
                        case 2:
                            x = PIECE_COORD_MAX - PIECES[piece].coords[i][0];
                            y = PIECE_COORD_MAX - PIECES[piece].coords[i][1];
                            break;
                        case 3:
                            x = PIECES[piece].coords[i][1];
                            y = PIECE_COORD_MAX - PIECES[piece].coords[i][0];
                            break;
                        default:
                            x = PIECES[piece].coords[i][0];
                            y = PIECES[piece].coords[i][1];
                    }
                    if (flipped) {
                        x = PIECE_COORD_MAX - x;
                    }
                    if (minX > x) {
                        minX = x;
                    }
                    if (minY > y) {
                        minY = y;
                    }
                    if (maxX < x) {
                        maxX = x;
                    }
                    if (maxY < y) {
                        maxY = y;
                    }
                    pieces[piece][rotation][flipped].coords[i][0] = x;
                    pieces[piece][rotation][flipped].coords[i][1] = y;
                }
                for (unsigned char i = 0; i < PIECES[piece].coord_count; ++i) {
                    pieces[piece][rotation][flipped].coords[i][0] -= minX;
                    pieces[piece][rotation][flipped].coords[i][1] -= minY;
                }
                pieces[piece][rotation][flipped].bounds[0] = maxX - minX;
                pieces[piece][rotation][flipped].bounds[1] = maxY - minY;
            }
        }
    }
}

std::vector<Move> GameState::getPossibleMoves(const std::function<bool(unsigned char)> &filter) {
    auto possibleMoves = std::vector<Move>{};
    Move move{static_cast<unsigned char>(turn % COLOR_COUNT)};

    if (turn < 4) {
        move.piece = firstPiece;
        for (; move.rotation < ROTATION_COUNT; ++move.rotation) {
            for (move.flipped = 0; move.flipped < FLIPPED_COUNT; ++move.flipped) {
                if (PIECE(pieces, move).coord_count == 0) {
                    continue;
                }
                unsigned char xo = 0;
                unsigned char yo = 0;
                if (turn == 1 || turn == 2) {
                    xo = PIECE(pieces, move).bounds[0];
                    move.x = BOARD_MAX - xo;
                }
                if (turn > 1) {
                    yo = PIECE(pieces, move).bounds[1];
                    move.y = BOARD_MAX - yo;
                }
                for (unsigned char i = 0; i < PIECE(pieces, move).coord_count; ++i) {
                    if (PIECE(pieces, move).coords[i][0] == xo && PIECE(pieces, move).coords[i][1] == yo) {
                        possibleMoves.push_back(move);
                        break;
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

    for (; move.piece < PIECE_COUNT; ++move.piece) {
        if (filter(move.piece) || deployedPieces[move.color][move.piece]) {
            continue;
        }
        for (move.rotation = 0; move.rotation < ROTATION_COUNT; ++move.rotation) {
            for (move.flipped = 0; move.flipped < FLIPPED_COUNT; ++move.flipped) {
                if (PIECE(pieces, move).coord_count == 0) {
                    continue;
                }
                for (move.x = 0; move.x < BOARD_SIZE - PIECE(pieces, move).bounds[0]; ++move.x) {
                    for (move.y = 0; move.y < BOARD_SIZE - PIECE(pieces, move).bounds[1]; ++move.y) {
                        bool valid = true;
                        bool diagonal = false;
                        unsigned char i = 0;
                        for (; i < PIECE(pieces, move).coord_count; ++i) {
                            unsigned char x = PIECE(pieces, move).coords[i][0];
                            unsigned char y = PIECE(pieces, move).coords[i][1];
                            unsigned char color = move.color + 1;
                            if (board[move.x + x][move.y + y] ||
                                (move.x + x + 1 <= BOARD_MAX && board[move.x + x + 1][move.y + y] == color) ||
                                (move.x + x - 1 >= 0 && board[move.x + x - 1][move.y + y] == color) ||
                                (move.y + y + 1 <= BOARD_MAX && board[move.x + x][move.y + y + 1] == color) ||
                                (move.y + y - 1 >= 0 && board[move.x + x][move.y + y - 1] == color)) {
                                valid = false;
                                break;
                            }
                            if (!diagonal && ((move.x + x + 1 <= BOARD_MAX && move.y + y + 1 <= BOARD_MAX &&
                                               board[move.x + x + 1][move.y + y + 1] == color) ||
                                              (move.x + x + 1 <= BOARD_MAX && move.y + y - 1 >= 0 &&
                                               board[move.x + x + 1][move.y + y - 1] == color) ||
                                              (move.x + x - 1 >= 0 && move.y + y + 1 <= BOARD_MAX &&
                                               board[move.x + x - 1][move.y + y + 1] == color) ||
                                              (move.x + x - 1 >= 0 && move.y + y - 1 >= 0 &&
                                               board[move.x + x - 1][move.y + y - 1] == color))) {
                                diagonal = true;
                            }
                            if (!diagonal && i + 1 == PIECE(pieces, move).diagonal_coord_count) {
                                valid = false;
                                break;
                            }
                        }
                        if (valid) {
                            possibleMoves.push_back(move);
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
    move.color++;
    if (move.piece < PIECE_COUNT) {
        for (unsigned char i = 0; i < PIECE(pieces, move).coord_count; ++i) {
            board[move.x + PIECE(pieces, move).coords[i][0]][move.y + PIECE(pieces, move).coords[i][1]] = move.color;
        }
        deployedPieces[move.color][move.piece] = 1;
    }
    turn++;
}

void GameState::undoMove(Move move) {
    if (move.piece < PIECE_COUNT) {
        for (unsigned char i = 0; i < PIECE(pieces, move).coord_count; ++i) {
            board[move.x + PIECE(pieces, move).coords[i][0]][move.y + PIECE(pieces, move).coords[i][1]] = 0;
        }
        deployedPieces[move.color][move.piece] = 0;
    }
    turn--;
}
