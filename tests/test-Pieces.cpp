#include <catch2/catch.hpp>
#include "../src/Pieces.h"

void cmpPair(U8 *pair, U8 a, U8 b) {
    REQUIRE(pair[0] == a);
    REQUIRE(pair[1] == b);
}

TEST_CASE("pieceBounds") {
    Pieces pieces{};

    SECTION("MONO") cmpPair(pieces.bounds[0], 0, 0);
    SECTION("DOMINO") cmpPair(pieces.bounds[1], 0, 1);
    SECTION("TRIO-L") cmpPair(pieces.bounds[2], 1, 1);
    SECTION("TRIO-I") cmpPair(pieces.bounds[3], 2, 0);
}

TEST_CASE("corners") {
    Pieces pieces{};

    SECTION("MONO") {
        REQUIRE(pieces.cornerCount[0] == 1);

        cmpPair(pieces.corners[0][0][0][0], 0, 0);
    }

    SECTION("DOMINO") {
        REQUIRE(pieces.cornerCount[1] == 2);

        cmpPair(pieces.corners[1][0][0][0], 0, 0);
        cmpPair(pieces.corners[1][0][0][1], 0, 1);

        cmpPair(pieces.corners[1][1][0][0], 0, 0);
        cmpPair(pieces.corners[1][1][0][1], 1, 0);
    }

    SECTION("TRIO-L") {
        REQUIRE(pieces.cornerCount[2] == 3);

        cmpPair(pieces.corners[2][0][0][0], 0, 0);
        cmpPair(pieces.corners[2][0][0][1], 1, 0);
        cmpPair(pieces.corners[2][0][0][2], 1, 1);

        cmpPair(pieces.corners[2][0][1][0], 0, 1);
        cmpPair(pieces.corners[2][0][1][1], 1, 0);
        cmpPair(pieces.corners[2][0][1][2], 1, 1);

        cmpPair(pieces.corners[2][1][0][0], 0, 0);
        cmpPair(pieces.corners[2][1][0][1], 0, 1);
        cmpPair(pieces.corners[2][1][0][2], 1, 0);

        cmpPair(pieces.corners[2][1][1][0], 0, 0);
        cmpPair(pieces.corners[2][1][1][1], 0, 1);
        cmpPair(pieces.corners[2][1][1][2], 1, 1);
    }

    SECTION("TRIO-I") {
        REQUIRE(pieces.cornerCount[3] == 2);

        cmpPair(pieces.corners[3][0][0][0], 0, 0);
        cmpPair(pieces.corners[3][0][0][1], 2, 0);

        cmpPair(pieces.corners[3][1][0][0], 0, 0);
        cmpPair(pieces.corners[3][1][0][1], 0, 2);
    }
}
