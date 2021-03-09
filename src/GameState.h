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
    unsigned int turn = 0;
    U32 board[COLOR_COUNT + 1][BOARD_SIZE]{};
    U64 boardHash = 0;
    U32 verticalNeighbours[COLOR_COUNT][BOARD_SIZE]{};
    U32 horizontalNeighbours[COLOR_COUNT][BOARD_SIZE]{};
    bool deployedPieces[COLOR_COUNT][PIECE_COUNT]{};
    bool gameOver[COLOR_COUNT]{};

    std::vector<Move> getPossibleMoves();

    void performMove(const Move &move);

    void undoMove(const Move &move);
};
