#pragma once

#include <stdint.h>
#include <chrono>

typedef uint8_t U8;
typedef uint32_t U32;
typedef uint64_t U64;

typedef std::chrono::time_point<std::chrono::system_clock> Time;

struct Move {
    U8 color, piece, rotation, flipped, x, y;
};

