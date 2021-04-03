#include <algorithm>
#include <iostream>
#include "Algorithm.h"
#include "Evaluation.h"
#include "Pieces.h"

bool Algorithm::checkTimeout() {
#ifndef NO_TIMEOUT
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= 1950) {
        timeout = true;
        return true;
    }
#endif
    return false;
}

std::vector<Move> Algorithm::sortedPossibleMoves(GameState &gameState) const {
    std::vector<Move> possibleMoves = gameState.getPossibleMoves();
    std::sort(possibleMoves.begin(), possibleMoves.end(), [this](Move a, Move b) {
        if (a.piece <= b.piece) return false;
        return evaluation.pieceEvaluation[a.piece] > evaluation.pieceEvaluation[b.piece];
    });
    return possibleMoves;
}

int Algorithm::alphaBeta(GameState &gameState, const int depth, int alpha, const int beta) {
    if (checkTimeout()) {
        return 0;
    }

    U64 hash = (gameState.boardHash << 2) + gameState.turn % 4;
    auto iterator = transpositions.find(hash);
    if (iterator != transpositions.end()) {
        Transposition transposition = iterator->second;
        if (transposition.depth >= depth) {
            if (transposition.bound == 0) return transposition.score;
            if (transposition.bound == 1 && transposition.score <= alpha) return alpha;
            if (transposition.bound == 2 && transposition.score >= beta) return beta;
        }
    }

    if (depth <= 0) return evaluation.evaluate(gameState);

    int bound = 1;

    for (auto move : sortedPossibleMoves(gameState)) {
        gameState.performMove(move);
        int score = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);

        if (timeout) return 0;

        if (score >= beta) {
            transpositions[hash] = {2, depth, beta};
            return beta;
        }

        if (score > alpha) {
            alpha = score;
            bound = 0;
        }
    }

    transpositions[hash] = {bound, depth, alpha};

    return alpha;
}

Move Algorithm::alphaBetaRoot(GameState &gameState, const int depth, int alpha, const int beta) {
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

#ifndef NO_OUTPUT
    printf("D%-2d S%-4d M{ %u %2u %u %u %2u %2u }\n", depth, alpha, bestMove.color, bestMove.piece,
           bestMove.rotation, bestMove.flipped, bestMove.x, bestMove.y);
#endif
    return bestMove;
}

Move Algorithm::iterativeDeepening(GameState &gameState) {
#ifndef NO_OUTPUT
    printf("       --- TURN %2d ---\n", gameState.turn);
#endif

    start = std::chrono::system_clock::now();
    timeout = false;

    Move move{};
    int depth;

    for (depth = 1; !timeout && depth <= 20; ++depth) {
        Move newMove = alphaBetaRoot(gameState, depth, -1000, 1000);
        if (!timeout || depth == 1) move = newMove;
    }

#ifndef NO_OUTPUT
    int time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
    printf("     --- TIME %4dms ---\n\n", time);
#endif
    return move;
}
