#include <set>
#include "Constants.h"

class GameState {
private:
    int turn = 0;
    unsigned char firstPiece = 11;
    unsigned char board[BOARD_SIZE][BOARD_SIZE]{};
    unsigned char pieces[PIECE_COUNT][ROTATION_COUNT][FLIPPED_COUNT][PIECE_COORD_COUNT + 2][COORD_COUNT]{};
    std::set<unsigned char> undeployedPieces[COLOR_COUNT];
public:
    GameState();

    std::vector<Move> *getPossibleMoves();

    unsigned char boardGet(unsigned char x, unsigned char y);

    void performMove(Move move);
};