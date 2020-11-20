#include "Algorithm.h"
#include "Evaluate.h"

int Algorithm::alphaBeta(int depth, int alpha, int beta) {
    if (depth == 0) return Evaluate::evaluate(gameState);

    for (Move move : *gameState->getPossibleMoves()) {
        gameState->performMove(move);
        int value = -alphaBeta(depth - 1, -beta, -alpha);
        gameState->undoMove(move);

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

Move Algorithm::run(GameState *gameState1) {
    this->gameState = gameState1;
    bestMove.color = 100;
    alphaBeta(initDepth, -1000000, 1000000);
    return bestMove;
}
