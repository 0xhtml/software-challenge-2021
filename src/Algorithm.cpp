#include <iostream>
#include "Algorithm.h"
#include "Evaluation.h"

int Algorithm::alphaBeta(GameState gameState, int depth, int alpha, int beta) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() >
        1900) {
        timeout = true;
        return 0;
    }

    auto iterator = transpositions.find(gameState.gameStateHash);
    if (iterator != transpositions.end()) {
        Transposition transposition = iterator->second;
        if (transposition.depth >= depth && ((transposition.value >= beta && transposition.bound != 2) ||
                                             (transposition.value < beta && transposition.bound != 1))) {
            return transposition.value;
        }
    }

    if (depth <= 0) return Evaluation::evaluate(gameState);

    int startAlpha = alpha;

    for (Move move : gameState.getPossibleMoves()) {
        gameState.performMove(move);
        int value = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);
        if (timeout) return 0;

        if (value >= beta) return beta;
        if (value > alpha) {
            alpha = value;
        }
    }

    transpositions[gameState.gameStateHash] = {alpha > beta ? 1 : alpha > startAlpha ? 2 : 0, depth, alpha};

    return alpha;
}

MoveValuePair Algorithm::alphaBetaRoot(GameState gameState, int depth, int alpha, int beta) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() >
        1900) {
        timeout = true;
        return {};
    }

    Move bestMove{};

    for (Move move : gameState.getPossibleMoves()) {
        gameState.performMove(move);
        int value = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);
        if (timeout) return {};

        if (value >= beta) return {{}, beta};
        if (value > alpha) {
            alpha = value;
            bestMove = move;
        }
    }

    return {bestMove, alpha};
}

MoveValuePair Algorithm::iterativeDeepening(GameState gameState) {
    start = std::chrono::system_clock::now();
    timeout = false;

    MoveValuePair bestValue{};
    int initDepth;

    for (initDepth = 1; initDepth < 20 && !timeout; ++initDepth) {
        MoveValuePair value = alphaBetaRoot(gameState, initDepth, -2147483640, 2147483640);
        if (!timeout) bestValue = value;
    }

    std::cout << "D" << initDepth - 1 << " V" << bestValue.value << std::endl;
    return bestValue;
}
