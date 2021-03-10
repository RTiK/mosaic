//
// Created by Artem Khatchatourov on 17.09.20.
//

#include <gtest/gtest.h>
#include "Piece.h"
#include "LabPiece.h"


TEST(PieceTests, DistanceTest) {
  auto p_1 = Piece(2, 1, 1);
  auto p_2 = Piece(0, 3, 2);
  auto distance = p_1.GetEuclideanDistance(p_2);
  EXPECT_EQ(distance, 3.0);
}
