#pragma once

#include "Constants.h"

const char *COLOR_NAMES[COLOR_COUNT] = {"BLUE", "YELLOW", "RED", "GREEN"};
const char *PIECE_NAMES[PIECE_COUNT] = {"MONO", "DOMINO", "TRIO_L", "TRIO_I", "TETRO_I", "TETRO_L", "TETRO_O",
                                        "TETRO_T", "TETRO_Z", "PENTO_I", "PENTO_U", "PENTO_L", "PENTO_V", "PENTO_P",
                                        "PENTO_W", "PENTO_R", "PENTO_X", "PENTO_Z", "PENTO_Y", "PENTO_T", "PENTO_S"};
const char *ROTATION_NAMES[ROTATION_COUNT] = {"NONE", "RIGHT", "MIRROR", "LEFT"};
const char *FLIPPED_NAMES[FLIPPED_COUNT] = {"false", "true"};

#define STR_INDEX(arr, str) std::distance(arr, std::find(arr, std::end(arr), std::string{str}))
