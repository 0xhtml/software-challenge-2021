#include "Algorithm.h"
#include <utility>

int Algorithm::alphaBeta(int depth, int alpha, int beta) {
    if (depth == 0) {
        int evaluation = 0;
        for (unsigned char x = 0; x < BOARD_SIZE; ++x) {
            for (unsigned char y = 0; y < BOARD_SIZE; ++y) {
                int value = gameState.boardGet(x, y);
                if (!value) continue;
                if (value == (gameState.getTurn() % 4) + 1 || value == ((gameState.getTurn() + 2) % 4) + 1) {
                    evaluation += 1;
                } else {
                    evaluation -= 1;
                }
            }
        }
        return evaluation;
    }

    for (Move move : gameState.getPossibleMoves()) {
        gameState.performMove(move);
        int value = -alphaBeta(depth - 1, -beta, -alpha);
        gameState.undoMove(move);

        if (value >= beta) return beta;
        if (value > alpha) {
            alpha = value;
            if (initDepth == depth) {
                bestMove = move;
            }
        }
    }

    return alpha;
}

Move Algorithm::run(GameState gameState1) {
    this->gameState = std::move(gameState1);
    bestMove.color = 100;
    alphaBeta(initDepth, -1000000, 1000000);
    return bestMove;
}
