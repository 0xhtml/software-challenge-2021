#ifndef SOFTWARE_CHALLENGE_2021_GAME_STATE_H
#define SOFTWARE_CHALLENGE_2021_GAME_STATE_H

#include <vector>
#include "Constants.h"
#include <functional>

class GameState {
private:
    unsigned char firstPiece = 11;
    Piece pieces[PIECE_COUNT][ROTATION_COUNT][FLIPPED_COUNT]{};
    unsigned char deployedPieces[COLOR_COUNT][PIECE_COUNT]{};
public:
    int turn = 0;
    unsigned char board[BOARD_SIZE][BOARD_SIZE]{};

    GameState();

    std::vector<Move> getPossibleMoves(const std::function<bool(unsigned char)> &filter);

    void performMove(Move move);

    void undoMove(Move move);
};

#endif
