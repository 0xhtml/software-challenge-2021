#pragma once

#include <vector>
#include "Constants.h"
#include "Hash.h"
#include "Types.h"

class GameState {
private:
    Hash hash{};
public:
    U8 firstPiece = 0;
    int turn = 0;
    U32 board[COLOR_COUNT + 1][BOARD_SIZE]{};
    U64 boardHash = 0;
    U64 verticalNeighbours[COLOR_COUNT][BOARD_SIZE]{};
    U64 horizontalNeighbours[COLOR_COUNT][BOARD_SIZE]{};
    bool deployedPieces[COLOR_COUNT][PIECE_COUNT]{};

    std::vector<Move> getPossibleMoves();

    void performMove(Move move);

    void undoMove(Move move);
};
