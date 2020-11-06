//
// Created by Artem Khatchatourov on 17.09.20.
//

#include <gtest/gtest.h>
#include "Piece.h"

TEST(PieceTests, DistanceTest) {
    auto p1 = Piece(2, 1, 1);
    auto p2 = Piece(0, 3, 2);
    auto distance = p1.getDistanceDelta76(&p2);
    EXPECT_EQ(distance, 3.0);
}


