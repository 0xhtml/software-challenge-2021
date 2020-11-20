#include <vector>
#include "GameState.h"

GameState::GameState() {
    for (unsigned char piece = 0; piece < PIECE_COUNT; ++piece) {
        for (unsigned char rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
            for (unsigned char flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                auto transformedPiece = new unsigned char[PIECE_COORD_COUNT][PIECE_COORD_COUNT]{};
                unsigned char minX = PIECE_COORD_COUNT;
                unsigned char minY = PIECE_COORD_COUNT;
                unsigned char maxX = 0;
                unsigned char maxY = 0;
                for (unsigned char x = 0; x < PIECE_COORD_COUNT; ++x) {
                    for (unsigned char y = 0; y < PIECE_COORD_COUNT; ++y) {
                        unsigned char x2, y2;
                        switch (rotation) {
                            case 1:
                                x2 = y;
                                y2 = PIECE_COORD_MAX - x;
                                break;
                            case 2:
                                x2 = PIECE_COORD_MAX - x;
                                y2 = PIECE_COORD_MAX - y;
                                break;
                            case 3:
                                x2 = PIECE_COORD_MAX - y;
                                y2 = x;
                                break;
                            default:
                                x2 = x;
                                y2 = y;
                        }
                        if (flipped) {
                            x2 = PIECE_COORD_MAX - x2;
                        }
                        if (PIECES[piece][x2][y2]) {
                            transformedPiece[x][y] = 1;
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
                        }
                    }
                }
                for (unsigned char x = 0; x < PIECE_COORD_COUNT - minX; ++x) {
                    for (unsigned char y = 0; y < PIECE_COORD_COUNT - minY; ++y) {
                        pieces[piece][rotation][flipped][x][y] = transformedPiece[x + minX][y + minY];
                    }
                }
                pieces[piece][rotation][flipped][PIECE_COORD_COUNT][0] = maxX - minX;
                pieces[piece][rotation][flipped][PIECE_COORD_COUNT][1] = maxY - minY;
            }
        }
    }
}

std::vector<Move> *GameState::getPossibleMoves() {
    auto possibleMoves = new std::vector<Move>();
    Move move{static_cast<unsigned char>(turn % COLOR_COUNT)};

    if (turn < 4) {
        move.piece = firstPiece;
        for (; move.rotation < ROTATION_COUNT; ++move.rotation) {
            for (move.flipped = 0; move.flipped < FLIPPED_COUNT; ++move.flipped) {
                unsigned char xo = 0;
                unsigned char yo = 0;
                if (turn == 1 || turn == 2) {
                    xo = PIECE_X(pieces, move)[0];
                    move.x = BOARD_MAX - xo;
                }
                if (turn > 1) {
                    yo = PIECE_X(pieces, move)[1];
                    move.y = BOARD_MAX - yo;
                }
                if (pieces[move.piece][move.rotation][move.flipped][xo][yo]) {
                    possibleMoves->push_back(move);
                }
            }
        }
        return possibleMoves;
    }

    for (; move.piece < PIECE_COUNT; ++move.piece) {
        for (move.rotation = 0; move.rotation < ROTATION_COUNT; ++move.rotation) {
            for (move.flipped = 0; move.flipped < FLIPPED_COUNT; ++move.flipped) {
                for (move.x = 0; move.x < BOARD_SIZE - PIECE_X(pieces, move)[0]; ++move.x) {
                    for (move.y = 0; move.y < BOARD_SIZE - PIECE_X(pieces, move)[1]; ++move.y) {
                        bool valid = true;
                        for (unsigned char x = 0; x <= PIECE_X(pieces, move)[0]; ++x) {
                            for (unsigned char y = 0; y <= PIECE_X(pieces, move)[1]; ++y) {
                                if (pieces[move.piece][move.rotation][move.flipped][x][y] == 0) {
                                    continue;
                                }
                                unsigned char color = move.color + 1;
                                if (board[move.x + x][move.y + y] || board[move.x + x + 1][move.y + y] == color ||
                                    board[move.x + x - 1][move.y + y] == color ||
                                    board[move.x + x][move.y + y + 1] == color ||
                                    board[move.x + x][move.y + y - 1] == color ||
                                    !(board[move.x + x + 1][move.y + y + 1] == color ||
                                      board[move.x + x + 1][move.y + y - 1] == color ||
                                      board[move.x + x - 1][move.y + y + 1] == color ||
                                      board[move.x + x - 1][move.y + y - 1] == color)) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (!valid) {
                                break;
                            }
                        }
                        if (valid) {
                            possibleMoves->push_back(move);
                        }
                    }
                }
            }
        }
    }

    return possibleMoves;
}

void GameState::performMove(Move move) {
    PRINT_MOVE(move);
    for (unsigned char x = 0; x < PIECE_COORD_COUNT; ++x) {
        for (unsigned char y = 0; y < PIECE_COORD_COUNT; ++y) {
            if (pieces[move.piece][move.rotation][move.flipped][x][y]) {
                board[move.x + x][move.y + y] = move.color + 1;
            }
        }
    }
    turn++;
}

unsigned char GameState::boardGet(unsigned char x, unsigned char y) {
    return board[x][y];
}
