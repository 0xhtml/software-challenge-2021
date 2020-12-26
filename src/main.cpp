#include <iostream>
#include <vector>
#include "Algorithm.h"
#include "GameState.h"
#include "Types.h"

void print_board(GameState gameState) {
    std::cout << std::endl;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (gameState.board[0][x] & 1 << y) {
                for (int color = 0; color < COLOR_COUNT; ++color) {
                    if (gameState.board[color + 1][x] & 1 << y) {
                        std::cout << color << " ";
                        break;
                    }
                }
            } else {
                std::cout << "· ";
            }
        }
        for (int color = 0; color < COLOR_COUNT; ++color) {
            std::cout << "  ";
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (gameState.board[color + 1][x] & 1 << y) {
                    std::cout << color << " ";
                } else {
                    std::cout << "· ";
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    GameState gameState = GameState{};
    Algorithm algorithm = Algorithm{};

    for (int i = 0; i < 40; ++i) {
        std::vector<Move> possibleMoves = gameState.getPossibleMoves();
        gameState.performMove(possibleMoves[rand() % possibleMoves.size()]);
        gameState.performMove(algorithm.run(gameState));
    }
    print_board(gameState);

    return 0;
}
