#include <catch2/catch.hpp>
#include "../src/GameState.h"

#define MOVE_TURN_0 {0, 1, 0, 0, 0, 0}
#define MOVE_TURN_1 {1, 1, 0, 0, BOARD_MAX, BOARD_MAX - 1}
#define MOVE_TURN_2 {2, 1, 0, 0, 0, BOARD_MAX - 1}
#define MOVE_TURN_3 {3, 1, 0, 0, BOARD_MAX, 0}
#define MOVE_TURN_4 {0, 8, 0, 0, 1, 2}

void checkPossibleMoves(GameState &gameState, const std::vector<Move> &moves) {
    std::vector<Move> possibleMoves = gameState.getPossibleMoves();

    REQUIRE(possibleMoves.size() == moves.size());

    for (int i = 0; i < possibleMoves.size(); ++i) {
        REQUIRE(possibleMoves[i].color == moves[i].color);
        REQUIRE(possibleMoves[i].piece == moves[i].piece);
        REQUIRE(possibleMoves[i].rotation == moves[i].rotation);
        REQUIRE(possibleMoves[i].flipped == moves[i].flipped);
        REQUIRE(possibleMoves[i].x == moves[i].x);
        REQUIRE(possibleMoves[i].y == moves[i].y);
    }
}

TEST_CASE("nextPossibleMove") {
    GameState gameState{};
    gameState.firstPiece = 1;

    SECTION("turn 0") {
        checkPossibleMoves(gameState, {
                {0, 1, 0, 0, 0, 0},
                {0, 1, 1, 0, 0, 0}
        });
    }

    gameState.performMove(MOVE_TURN_0);

    SECTION("turn 1") {
        checkPossibleMoves(gameState, {
                {1, 1, 0, 0, BOARD_MAX, BOARD_MAX - 1},
                {1, 1, 1, 0, BOARD_MAX - 1, BOARD_MAX}
        });
    }

    gameState.performMove(MOVE_TURN_1);

    SECTION("turn 2") {
        checkPossibleMoves(gameState, {
                {2, 1, 0, 0, 0, BOARD_MAX - 1},
                {2, 1, 1, 0, 0, BOARD_MAX}
        });
    }

    gameState.performMove(MOVE_TURN_2);

    SECTION("turn 3") {
        checkPossibleMoves(gameState, {
                {3, 1, 0, 0, BOARD_MAX, 0},
                {3, 1, 1, 0, BOARD_MAX - 1, 0}
        });
    }

    gameState.performMove(MOVE_TURN_3);

    SECTION("turn 4") {
        // TODO: Verify this is the actual number of possibleMoves
        REQUIRE(gameState.getPossibleMoves().size() == 113);
    }

    // TODO: Add more test sections for e.g. SkipMoves
}

void checkGameState(const GameState &gameState) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
        U32 board = 0;

        for (int color = 0; color < COLOR_COUNT; ++color) {
            U32 verticalNeighbours = gameState.board[color + 1][x] << 1 | gameState.board[color + 1][x] >> 1;
            REQUIRE(gameState.verticalNeighbours[color][x] == verticalNeighbours);

            U32 horizontalNeighbours = 0;
            if (x > 0) horizontalNeighbours |= gameState.board[color + 1][x - 1];
            if (x < BOARD_MAX) horizontalNeighbours |= gameState.board[color + 1][x + 1];
            REQUIRE(gameState.horizontalNeighbours[color][x] == horizontalNeighbours);

            board |= gameState.board[color + 1][x];
        }

        REQUIRE(gameState.board[0][x] == board);
    }
}

TEST_CASE("performMove and undoMove") {
    GameState gameState{};

    gameState.performMove(MOVE_TURN_0);
    gameState.performMove(MOVE_TURN_1);
    gameState.performMove(MOVE_TURN_2);
    gameState.performMove(MOVE_TURN_3);
    gameState.performMove(MOVE_TURN_4);

    REQUIRE(gameState.turn == 5);
    checkGameState(gameState);

    gameState.undoMove(MOVE_TURN_4);

    REQUIRE(gameState.turn == 4);
    checkGameState(gameState);
}
