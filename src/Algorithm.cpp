#include <algorithm>
#include <iostream>
#include "Algorithm.h"
#include "Evaluation.h"

bool compareMoves(Move a, Move b) {
    if (a.piece == b.piece)
        return Evaluation::evaluateCoords(a.x, a.y) > Evaluation::evaluateCoords(b.x, b.y);
    return PIECE_SCORES[a.piece] > PIECE_SCORES[b.piece];
}

std::vector<Move> sortedPossibleMoves(GameState gameState) {
    std::vector<Move> possibleMove = gameState.getPossibleMoves();
    std::sort(possibleMove.begin(), possibleMove.end(), compareMoves);
    return possibleMove;
}

int Algorithm::alphaBeta(GameState gameState, int depth, int alpha, int beta) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() >
        1900) {
        timeout = true;
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

    if (depth <= 0) return Evaluation::evaluate(gameState);

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

MoveScorePair Algorithm::alphaBetaRoot(GameState gameState, int depth, int alpha, int beta) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() >
        1900) {
        timeout = true;
        return {};
    }

    Move bestMove{};

    for (Move move : sortedPossibleMoves(gameState)) {
        gameState.performMove(move);
        int score = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);
        if (timeout) return {};

        if (score >= beta) return {{5}, beta};
        if (score > alpha) {
            alpha = score;
            bestMove = move;
        }
    }

    return {bestMove, alpha};
}

MoveScorePair Algorithm::iterativeDeepening(GameState gameState) {
    start = std::chrono::system_clock::now();
    timeout = false;

    MoveScorePair bestScore{};
    int initDepth;

    for (initDepth = 1; initDepth < 20 && !timeout; ++initDepth) {
        MoveScorePair score = alphaBetaRoot(gameState, initDepth, -2147483640, 2147483640);
        if (!timeout) bestScore = score;
    }

    std::cout << "D" << initDepth - 1 << " V" << bestScore.score << std::endl;
    return bestScore;
}
