#include <algorithm>
#include <iostream>
#include "Algorithm.h"
#include "Evaluation.h"
#include "Pieces.h"

bool Algorithm::checkTimeout() {
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > 1900) {
        timeout = true;
        return true;
    }
    return false;
}

std::vector<Move> Algorithm::sortedPossibleMoves(GameState gameState) {
    std::vector<Move> possibleMove = gameState.getPossibleMoves();
    std::sort(possibleMove.begin(), possibleMove.end(), [this](Move a, Move b) {
        return evaluation.pieceEvaluation[a.piece] > evaluation.pieceEvaluation[b.piece];
    });
    return possibleMove;
}

int Algorithm::alphaBeta(GameState gameState, int depth, int alpha, int beta) {
    if (checkTimeout()) {
        return 0;
    }

    U64 hash = (gameState.boardHash << 2) + gameState.turn % 4;
    auto iterator = transpositions.find(hash);
    if (iterator != transpositions.end()) {
        Transposition transposition = iterator->second;
        if (transposition.depth >= depth && ((transposition.score >= beta && transposition.bound != 2) ||
                                             (transposition.score < beta && transposition.bound != 1))) {
            return transposition.score;
        }
    }

    if (depth <= 0) return evaluation.evaluate(gameState);

    int startAlpha = alpha;
    for (auto move : sortedPossibleMoves(gameState)) {
        gameState.performMove(move);
        int score = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);

        if (timeout) return 0;
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    transpositions[hash] = {alpha > beta ? 1 : alpha > startAlpha ? 2 : 0, depth, alpha};

    return alpha;
}

Move Algorithm::alphaBetaRoot(GameState gameState, int depth, int alpha, int beta) {
    if (checkTimeout()) {
        return {5};
    }

    Move bestMove{5};

    for (Move move : sortedPossibleMoves(gameState)) {
        gameState.performMove(move);
        int score = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);
        if (timeout) return bestMove;

        if (score >= beta) return move;
        if (score > alpha) {
            alpha = score;
            bestMove = move;
        }
    }

    return bestMove;
}

Move Algorithm::iterativeDeepening(GameState gameState) {
    start = std::chrono::system_clock::now();
    timeout = false;

    Move move{};
    int depth;

    for (depth = 1; !timeout && depth <= 20; ++depth) {
        Move newMove = alphaBetaRoot(gameState, depth, -1000, 1000);
        if (!timeout || depth == 1) move = newMove;
    }

    std::cout << "D" << depth - 1 - timeout << std::endl;
    return move;
}
