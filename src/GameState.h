#pragma once

#include <vector>

#include "Constants.h"
#include "Hash.h"
#include "Pieces.h"
#include "Types.h"

class GameState {
private:
    Hash hash{};
    Pieces pieces{};

    U32 verticalNeighbours[COLOR_COUNT][BOARD_SIZE]{};
    U32 horizontalNeighbours[COLOR_COUNT][BOARD_SIZE]{};

public:
    int turn = 0;
    U8 firstPiece = 0;

    U32 board[COLOR_COUNT][BOARD_SIZE]{};
    U32 boardOR[BOARD_SIZE]{};

    U64 boardHash = 0;

    bool deployedPieces[COLOR_COUNT][PIECE_COUNT]{};

    U32 getValidFields(const int color, const int x) const;

    std::vector<Move> getPossibleMoves() const;

    void performMove(const Move &move);

    void undoMove(const Move &move);
};
