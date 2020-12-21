#include <iostream>
#include "Algorithm.h"

bool filter_piece(unsigned char piece) {
    return piece < 9;
}

int Algorithm::alphaBeta(GameState gameState, int depth, int alpha, int beta) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() >
        1900) {
        timeout = true;
        return 0;
    }

    unsigned long int gameStateHash = hash.hash(gameState);

    auto iterator = transpositions.find(gameStateHash);
    if (iterator != transpositions.end()) {
        Transposition transposition = iterator->second;
        if (transposition.control == gameState.boardGet(MOST_SIGNIFICANT_X, MOST_SIGNIFICANT_Y) &&
            transposition.depth >= depth && ((transposition.value >= beta && transposition.bound != 2) ||
                                             (transposition.value < beta && transposition.bound != 1))) {
            return transposition.value;
        }
    }

    if (depth == 0) return Evaluation::evaluate(gameState);

    int start_alpha = alpha;

    for (Move move : gameState.getPossibleMoves(filter_piece)) {
        gameState.performMove(move);
        int value = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        if (timeout) return 0;
        gameState.undoMove(move);

        if (value >= beta) return beta;
        if (value > alpha) {
            alpha = value;
            if (initDepth == depth) bestMove = move;
        }
    }

    transpositions[gameStateHash] = {gameState.boardGet(MOST_SIGNIFICANT_X, MOST_SIGNIFICANT_Y),
                                     static_cast<unsigned char>(alpha > beta ? 1 : alpha > start_alpha ? 2 : 0), depth,
                                     alpha};

    return alpha;
}

Move Algorithm::run(GameState gameState) {
    start = std::chrono::system_clock::now();
    timeout = false;
    bestMove.piece = 250;

    for (initDepth = 1; initDepth < 20 && !timeout; initDepth++)
        alphaBeta(gameState, initDepth, -1000000, 1000000);
    std::cout << "Depth: " << initDepth - 1 << std::endl;

    return bestMove;
}
