#include "Algorithm.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <vector>

#include "GameState.h"
#include "Types.h"

Algorithm::Algorithm() {
#ifndef TESTING
    transpositions.rehash(2700023);
    transpositions.max_load_factor(-1);
#endif
}

bool Algorithm::checkTimeout() {
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= 1950) {
        timeout = true;
        return true;
    }
    return false;
}

std::vector<Move> Algorithm::sortedPossibleMoves(GameState &gameState) const {
    std::vector<Move> possibleMoves = gameState.getPossibleMoves();
    std::sort(possibleMoves.begin(), possibleMoves.end(), [this](Move a, Move b) {
        if (a.piece != b.piece) {
            int diff = evaluation.pieceSize[a.piece] - evaluation.pieceSize[b.piece];
            if (diff != 0) return diff > 0;
        }
        return history[a.piece][a.rotation][a.flipped][a.x][a.y] > history[b.piece][b.rotation][b.flipped][b.x][b.y];
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
            if (transposition.type == EXACT) return transposition.score;
            if (transposition.type == ALPHA && transposition.score <= alpha) return alpha;
            if (transposition.type == BETA && transposition.score >= beta) return beta;
        }
    }

    if (depth <= 0) return evaluation.evaluate(gameState);

    TranspositionType type = ALPHA;

    for (auto move : sortedPossibleMoves(gameState)) {
        gameState.performMove(move);
        int score = -alphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move);

        if (timeout) return 0;

        if (score >= beta) {
            transpositions[hash] = {BETA, depth, beta};
            ++history[move.piece][move.rotation][move.flipped][move.x][move.y];
            return beta;
        }

        if (score > alpha) {
            alpha = score;
            type = EXACT;
        }
    }

    transpositions[hash] = {type, depth, alpha};

    return alpha;
}

Move Algorithm::alphaBetaRoot(GameState &gameState, const int depth, int alpha, const int beta) {
    if (checkTimeout()) {
        return {5};
    }

    Move bestMove{5};

    for (Move move : gameState.getPossibleMoves()) {
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

#ifndef TESTING
    printf("D%-2d S%-4d M{ %u %2u %u %u %2u %2u }\n", depth, alpha, bestMove.color, bestMove.piece,
           bestMove.rotation, bestMove.flipped, bestMove.x, bestMove.y);
#endif
    return bestMove;
}

Move Algorithm::iterativeDeepening(GameState &gameState) {
#ifndef TESTING
    printf("       --- TURN %2d ---\n", gameState.turn);
#endif

    start = std::chrono::system_clock::now();
    timeout = false;

    Move move{};

    if (gameState.turn < 14) {
        move = alphaBetaRoot(gameState, 1, -1000, 1000);

#ifndef TESTING
        for (int depth = 15 - gameState.turn; !timeout; ++depth) {
            alphaBetaRoot(gameState, depth, -1000, 1000);
        }
#endif
    } else {
        for (int depth = 1; !timeout && depth <= 20; ++depth) {
            Move newMove = alphaBetaRoot(gameState, depth, -1000, 1000);
            if (!timeout || depth == 1) move = newMove;
        }
    }

#ifndef TESTING
    int time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
    printf("     --- TIME %4dms ---\n\n", time);
#endif
    return move;
}
