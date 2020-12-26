#ifndef SOFTWARE_CHALLENGE_2021_GAME_STATE_H
#define SOFTWARE_CHALLENGE_2021_GAME_STATE_H

#include <vector>
#include <algorithm>
#include "Constants.h"
#include "Types.h"

class GameState {
private:
    int firstPiece = 11;
    bool deployedPieces[COLOR_COUNT][PIECE_COUNT]{};
public:
    int turn = 0;
    U32 board[COLOR_COUNT + 1][BOARD_SIZE]{};

    std::vector<Move> getPossibleMoves();

    void performMove(Move move);

    void undoMove(Move move);
};

#endif
