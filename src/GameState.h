#ifndef SOFTWARE_CHALLENGE_2021_GAME_STATE_H
#define SOFTWARE_CHALLENGE_2021_GAME_STATE_H

#include <vector>
#include "Constants.h"

class GameState {
private:
    unsigned char firstPiece = 11;
    unsigned char deployedPieces[COLOR_COUNT][PIECE_COUNT]{};
public:
    int turn = 0;
    unsigned int board[COLOR_COUNT + 1][BOARD_SIZE]{};

    std::vector<Move> getPossibleMoves(bool(*filter)(unsigned char));

    void performMove(Move move);

    void undoMove(Move move);
};

#endif
