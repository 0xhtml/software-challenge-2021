#ifndef SOFTWARE_CHALLENGE_2021_GAME_STATE_H
#define SOFTWARE_CHALLENGE_2021_GAME_STATE_H

#include <vector>
#include "Constants.h"
#include <functional>

class GameState {
private:
    int turn = 0;
    unsigned char firstPiece = 11;
    unsigned char board[BOARD_SIZE][BOARD_SIZE]{};
    Piece pieces[PIECE_COUNT][ROTATION_COUNT][FLIPPED_COUNT]{};
    unsigned char deployedPieces[COLOR_COUNT][PIECE_COUNT]{};
public:
    GameState();

    std::vector<Move> getPossibleMoves(const std::function<bool(unsigned char)> &filter);

    unsigned char boardGet(unsigned char x, unsigned char y);

    void performMove(Move move);

    void undoMove(Move move);

    int getTurn() const;
};

#endif
