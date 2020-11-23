#include "Algorithm.h"

bool filter_piece(unsigned char piece) {
    return piece < 9;
}

int Algorithm::alphaBeta(GameState gameState, int depth, int alpha, int beta) {
    if (depth == 0) return evaluation.evaluate(gameState);

    for (Move move : gameState.getPossibleMoves(filter_piece)) {
        gameState.performMove(move);
        int value = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);

        if (value >= beta) return beta;
        if (value > alpha) {
            alpha = value;
            if (initDepth == depth) bestMove = move;
        }
    }

    return alpha;
}

Move Algorithm::run(GameState gameState) {
    bestMove.color = 100;
    alphaBeta(gameState, initDepth, -1000000, 1000000);
    return bestMove;
}
