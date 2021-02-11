#include <algorithm>
#include <iostream>
#include "Algorithm.h"
#include "Evaluation.h"

bool Algorithm::checkTimeout() {
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > 1900) {
        timeout = true;
        return true;
    }
    return false;
}

bool compareMoves(Move a, Move b) {
    return PIECE_SCORES[a.piece] > PIECE_SCORES[b.piece];
}

std::vector<Move> sortedPossibleMoves(GameState gameState) {
    std::vector<Move> possibleMove = gameState.getPossibleMoves();
    std::sort(possibleMove.begin(), possibleMove.end(), compareMoves);
    return possibleMove;
}

int Algorithm::alphaBeta(GameState gameState, int depth, int alpha, int beta) {
    if (checkTimeout()) {
        return 0;
    }

    int preBestMoveId = 0;
    auto iterator = transpositions.find(gameState.gameStateHash);
    if (iterator != transpositions.end()) {
        Transposition transposition = iterator->second;
        if (transposition.depth >= depth && ((transposition.score >= beta && transposition.bound != 2) ||
                                             (transposition.score < beta && transposition.bound != 1))) {
            return transposition.score;
        }
        preBestMoveId = transposition.bestMoveId;
    }

    if (depth <= 0) return gameState.evaluation * (gameState.turn % 2 ? -1 : 1);

    std::vector<Move> possibleMoves = sortedPossibleMoves(gameState);

    Move move = possibleMoves[preBestMoveId];
    gameState.performMove(move);
    int score = -alphaBeta(gameState, depth - 1, -beta, -alpha);
    gameState.undoMove(move);
    if (timeout) return 0;

    if (score >= beta) return beta;

    int startAlpha = alpha;
    if (score > alpha) {
        alpha = score;
    }

    int bestMoveId = preBestMoveId;
    for (int i = 0; i < possibleMoves.size(); i++) {
        if (i == preBestMoveId) continue;
        move = possibleMoves[i];

        gameState.performMove(move);
        score = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);
        if (timeout) return 0;

        if (score >= beta) return beta;
        if (score > alpha) {
            alpha = score;
            bestMoveId = i;
        }
    }

    transpositions[gameState.gameStateHash] = {alpha > beta ? 1 : alpha > startAlpha ? 2 : 0, depth, alpha, bestMoveId};

    return alpha;
}

Move Algorithm::alphaBetaRoot(GameState gameState, int depth, int alpha, int beta) {
    if (checkTimeout()) {
        return {};
    }

    Move bestMove{};

    for (Move move : sortedPossibleMoves(gameState)) {
        gameState.performMove(move);
        int score = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);
        if (timeout) return {};

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

    for (depth = 1; depth < 20 && !timeout; ++depth) {
        Move newMove = alphaBetaRoot(gameState, depth, -2147483640, 2147483640);
        if (!timeout) move = newMove;
    }

    std::cout << "D" << depth - 1 - timeout << std::endl;
    return move;
}
