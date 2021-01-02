#ifndef SOFTWARE_CHALLENGE_2021_GAME_STATE_H
#define SOFTWARE_CHALLENGE_2021_GAME_STATE_H

#include <vector>
#include "Constants.h"
#include "Hash.h"
#include "Types.h"

class GameState {
private:
    int firstPiece = 11;
    Hash hash{};
public:
    int turn = 0;
    U32 board[COLOR_COUNT + 1][BOARD_SIZE]{};
    U64 gameStateHash = 0;
    int evaluation = 0;
    U64 verticalNeighbours[COLOR_COUNT][BOARD_SIZE]{};

    std::vector<Move> getPossibleMoves();

    void performMove(Move move);

    void undoMove(Move move);

    bool deployedPieces[COLOR_COUNT][PIECE_COUNT]{};
};

#endif
