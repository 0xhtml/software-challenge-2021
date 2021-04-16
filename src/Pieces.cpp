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

        for (int rotation = 0; rotation < ROTATION_COUNT; ++rotation) {
            for (int flipped = 0; flipped < FLIPPED_COUNT; ++flipped) {
                if (PIECES[piece][rotation][flipped][0] == 0) break;

                int i = 0;
                for (U8 x = 0; x <= bounds[piece][rotation % 2 ? 1 : 0]; ++x) {
                    for (U8 y = 0; y <= bounds[piece][rotation % 2 ? 0 : 1]; ++y) {
                        if (!(PIECES[piece][rotation][flipped][x] & 1 << y)) continue;

                        if (x > 0 && PIECES[piece][rotation][flipped][x - 1] & 1 << y &&
                            x < PIECE_MAX && PIECES[piece][rotation][flipped][x + 1] & 1 << y ||
                            y > 0 && PIECES[piece][rotation][flipped][x] & 1 << (y - 1) &&
                            y < PIECE_MAX && PIECES[piece][rotation][flipped][x] & 1 << (y + 1)) continue;

                        corners[piece][rotation][flipped][i][0] = x;
                        corners[piece][rotation][flipped][i][1] = y;
                        ++i;
                    }
                }
                cornerCount[piece] = i;
            }
        }
    }
}
