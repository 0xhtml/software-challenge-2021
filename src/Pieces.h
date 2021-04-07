#pragma once

#include "Constants.h"
#include "Types.h"

constexpr U8 PIECES[PIECE_COUNT][ROTATION_COUNT][FLIPPED_COUNT][PIECE_SIZE] = {
    {{{0b1}}},
    {{{0b11}}, {{0b1, 0b1}}},
    {{{0b1, 0b11}, {0b10, 0b11}}, {{0b11, 0b1}, {0b11, 0b10}}},
    {{{0b1, 0b1, 0b1}}, {{0b111}}},
    {{{0b1, 0b1, 0b1, 0b1}}, {{0b1111}}},
    {{{0b1, 0b1, 0b11}, {0b10, 0b10, 0b11}}, {{0b111, 0b1}, {0b111, 0b100}}, {{0b11, 0b10, 0b10}, {0b11, 0b1, 0b1}}, {{0b100, 0b111}, {0b1, 0b111}}},
    {{{0b11, 0b11}}},
    {{{0b111, 0b10}}, {{0b10, 0b11, 0b10}, {0b1, 0b11, 0b1}}, {{0b10, 0b111}}},
    {{{0b11, 0b110}, {0b110, 0b11}}, {{0b10, 0b11, 0b1}, {0b1, 0b11, 0b10}}},
    {{{0b1, 0b1, 0b1, 0b1, 0b1}}, {{0b11111}}},
    {{{0b101, 0b111}}, {{0b11, 0b1, 0b11}, {0b11, 0b10, 0b11}}, {{0b111, 0b101}}},
    {{{0b1, 0b1, 0b1, 0b11}, {0b10, 0b10, 0b10, 0b11}}, {{0b1111, 0b1}, {0b1111, 0b1000}}, {{0b11, 0b10, 0b10, 0b10}, {0b11, 0b1, 0b1, 0b1}}, {{0b1000, 0b1111}, {0b1, 0b1111}}},
    {{{0b1, 0b1, 0b111}, {0b100, 0b100, 0b111}}, {{0b111, 0b1, 0b1}, {0b111, 0b100, 0b100}}},
    {{{0b11, 0b11, 0b1}, {0b11, 0b11, 0b10}}, {{0b111, 0b110}, {0b111, 0b11}}, {{0b10, 0b11, 0b11}, {0b1, 0b11, 0b11}}, {{0b11, 0b111}, {0b110, 0b111}}},
    {{{0b1, 0b11, 0b110}, {0b100, 0b110, 0b11}}, {{0b110, 0b11, 0b1}, {0b11, 0b110, 0b100}}},
    {{{0b100, 0b111, 0b10}, {0b1, 0b111, 0b10}}, {{0b10, 0b11, 0b110}, {0b10, 0b110, 0b11}}, {{0b10, 0b111, 0b1}, {0b10, 0b111, 0b100}}, {{0b11, 0b110, 0b10}, {0b110, 0b11, 0b10}}},
    {{{0b10, 0b111, 0b10}}},
    {{{0b11, 0b10, 0b110}, {0b110, 0b10, 0b11}}, {{0b100, 0b111, 0b1}, {0b1, 0b111, 0b100}}},
    {{{0b10, 0b11, 0b10, 0b10}, {0b1, 0b11, 0b1, 0b1}}, {{0b100, 0b1111}, {0b10, 0b1111}}, {{0b1, 0b1, 0b11, 0b1}, {0b10, 0b10, 0b11, 0b10}}, {{0b1111, 0b10}, {0b1111, 0b100}}},
    {{{0b111, 0b10, 0b10}}, {{0b100, 0b111, 0b100}, {0b1, 0b111, 0b1}}, {{0b10, 0b10, 0b111}}},
    {{{0b1110, 0b11}, {0b111, 0b1100}}, {{0b1, 0b11, 0b10, 0b10}, {0b10, 0b11, 0b1, 0b1}}, {{0b1100, 0b111}, {0b11, 0b1110}}, {{0b1, 0b1, 0b11, 0b10}, {0b10, 0b10, 0b11, 0b1}}}
};

class Pieces {
public:
    U8 pieceBounds[PIECE_COUNT][2]{};

    Pieces();
};
