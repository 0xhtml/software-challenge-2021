#include "Pieces.h"
#include "Constants.h"
#include "Types.h"

Pieces::Pieces() {
    for (int piece = 0; piece < PIECE_COUNT; ++piece) {
        int x = 0;
        int y = 0;

        for (; x < PIECE_SIZE; ++x) {
            if (PIECES[piece][0][0][x] == 0) break;

            U8 a = PIECES[piece][0][0][x] >> y;
            while (a) {
                ++y;
                a >>= 1;
            }
        }

        bounds[piece][0] = x - 1;
        bounds[piece][1] = y - 1;
    }
}
