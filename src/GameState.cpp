#include "GameState.h"

GameState::GameState() {
    for (unsigned char piece = 0; piece < PIECE_COUNT; ++piece) {
        for (unsigned char rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
            for (unsigned char flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                pieces[piece][rotation][flipped][0][0] = PIECES[piece][0][0];
                pieces[piece][rotation][flipped][0][1] = PIECES[piece][0][1];
                unsigned char minX = PIECE_COORD_COUNT;
                unsigned char minY = PIECE_COORD_COUNT;
                unsigned char maxX = 0;
                unsigned char maxY = 0;
                for (unsigned char i = 0; i < PIECES[piece][0][0]; ++i) {
                    unsigned char x, y;
                    switch (rotation) {
                        case 1:
                            x = PIECE_COORD_MAX - PIECES[piece][i + 1][1];
                            y = PIECES[piece][i + 1][0];
                            break;
                        case 2:
                            x = PIECE_COORD_MAX - PIECES[piece][i + 1][0];
                            y = PIECE_COORD_MAX - PIECES[piece][i + 1][1];
                            break;
                        case 3:
                            x = PIECES[piece][i + 1][1];
                            y = PIECE_COORD_MAX - PIECES[piece][i + 1][0];
                            break;
                        default:
                            x = PIECES[piece][i + 1][0];
                            y = PIECES[piece][i + 1][1];
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
                    pieces[piece][rotation][flipped][i + 1][0] = x;
                    pieces[piece][rotation][flipped][i + 1][1] = y;
                }
                for (unsigned char i = 0; i < PIECES[piece][0][0]; ++i) {
                    pieces[piece][rotation][flipped][i + 1][0] -= minX;
                    pieces[piece][rotation][flipped][i + 1][1] -= minY;
                }
                pieces[piece][rotation][flipped][6][0] = maxX - minX;
                pieces[piece][rotation][flipped][6][1] = maxY - minY;
            }
        }
    }
    for (auto &piece : pieces) {
        for (auto & rotation : piece) {
            for (auto & flipped : rotation) {
                if (flipped[0][0] == 0) {
                    continue;
                }
                for (auto & rotation2 : piece) {
                    for (auto & flipped2 : rotation2) {
                        if (flipped == flipped2 || flipped2[0][0] == 0) {
                            continue;
                        }
                        for (int i = 0; i < flipped[0][0]; ++i) {
                            if (flipped[i + 1][0] == flipped2[i + 1][0] && flipped[i + 1][1] == flipped2[i + 1][1]) {
                                flipped2[0][0] = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

std::vector<Move> GameState::getPossibleMoves() {
    auto possibleMoves = std::vector<Move>{};
    Move move{static_cast<unsigned char>(turn % COLOR_COUNT)};

    if (turn < 4) {
        move.piece = firstPiece;
        for (; move.rotation < ROTATION_COUNT; ++move.rotation) {
            for (move.flipped = 0; move.flipped < FLIPPED_COUNT; ++move.flipped) {
                unsigned char xo = 0;
                unsigned char yo = 0;
                if (turn == 1 || turn == 2) {
                    xo = PIECE(pieces, move)[6][0];
                    move.x = BOARD_MAX - xo;
                }
                if (turn > 1) {
                    yo = PIECE(pieces, move)[6][1];
                    move.y = BOARD_MAX - yo;
                }
                for (unsigned char i = 0; i < PIECE(pieces, move)[0][0]; ++i) {
                    if (PIECE(pieces, move)[i + 1][0] == xo && PIECE(pieces, move)[i + 1][1] == yo) {
                        possibleMoves.push_back(move);
                        break;
                    }
                }
            }
        }
        return possibleMoves;
    }

    for (; move.piece < PIECE_COUNT; ++move.piece) {
        if (deployedPieces[move.color][move.piece]) {
            continue;
        }
        for (move.rotation = 0; move.rotation < ROTATION_COUNT; ++move.rotation) {
            for (move.flipped = 0; move.flipped < FLIPPED_COUNT; ++move.flipped) {
                if (PIECE(pieces, move)[0][0] == 0) {
                    continue;
                }
                for (move.x = 0; move.x < BOARD_SIZE - PIECE(pieces, move)[6][0]; ++move.x) {
                    for (move.y = 0; move.y < BOARD_SIZE - PIECE(pieces, move)[6][1]; ++move.y) {
                        bool valid = true;
                        bool diagonal = false;
                        unsigned char i = 0;
                        for (; i < PIECE(pieces, move)[0][0]; ++i) {
                            unsigned char x = PIECE(pieces, move)[i + 1][0];
                            unsigned char y = PIECE(pieces, move)[i + 1][1];
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
                            if (!diagonal && i + 1 == PIECE(pieces, move)[0][1]) {
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

    return possibleMoves;
}

void GameState::performMove(Move move) {
    for (unsigned char i = 0; i < PIECE(pieces, move)[0][0]; ++i) {
        board[move.x + PIECE(pieces, move)[i + 1][0]][move.y + PIECE(pieces, move)[i + 1][1]] = move.color + 1;
    }
    deployedPieces[move.color][move.piece] = 1;
    turn++;
}

unsigned char GameState::boardGet(unsigned char x, unsigned char y) {
    return board[x][y];
}

void GameState::undoMove(Move move) {
    for (unsigned char i = 0; i < PIECE(pieces, move)[0][0]; ++i) {
        board[move.x + PIECE(pieces, move)[i + 1][0]][move.y + PIECE(pieces, move)[i + 1][1]] = 0;
    }
    deployedPieces[move.color][move.piece] = 0;
    turn--;
}

int GameState::getTurn() const {
    return turn;
}
