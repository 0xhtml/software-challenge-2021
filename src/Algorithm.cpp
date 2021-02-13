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

bool compareMoves(Move a, Move b) {
    int value = 0;
    for (int x = 0; x < PIECE_SIZE; ++x) {
        if (PIECES[a.piece][0][0][x] == 0 && PIECES[b.piece][0][0][x] == 0) break;
        value += __builtin_popcount(PIECES[a.piece][0][0][x]);
        value -= __builtin_popcount(PIECES[b.piece][0][0][x]);
    }
    return value > 0;
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

    U64 hash = (gameState.boardHash << 2) + gameState.turn % 4;
    auto iterator = transpositions.find(hash);
    if (iterator != transpositions.end()) {
        Transposition transposition = iterator->second;
        if (transposition.depth >= depth && ((transposition.score >= beta && transposition.bound != 2) ||
                                             (transposition.score < beta && transposition.bound != 1))) {
            return transposition.score;
        }
    }

    if (depth <= 0) return Evaluation::evaluate(gameState);

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

    for (depth = 1; !timeout && gameState.turn + depth <= TURN_LIMIT; ++depth) {
        Move newMove = alphaBetaRoot(gameState, depth, -2147483640, 2147483640);
        if (!timeout) move = newMove;
    }

    std::cout << "D" << depth - 1 - timeout << std::endl;
    return move;
}
