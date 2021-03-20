//
// Created by Artem Khatchatourov on 17.09.20.
//

#include <gtest/gtest.h>
#include "Piece.h"
#include "LabPiece.h"


TEST(PieceTests, DistanceTest) {
  auto p_1 = Piece(0.2, 0.1, 0.1);
  auto p_2 = Piece(0.0, 0.3, 0.2);
  auto distance = p_1.GetEuclideanDistance(p_2);
  EXPECT_NEAR(distance, 0.3, 0.00001);
}


/**
 * Tests that LabPiece stores and returns BGR values via getter methods but internally the distance is evaluated
 * using CIELAB.
 */
TEST(PieceTests, LabAssignmentTest) {
  auto lab_piece_1 = LabPiece(0.5, 0.5, 0.5);
  auto lab_piece_2 = LabPiece(0.6, 0.6, 0.6);
  ColorT c_1 = lab_piece_1.GetColor();
  ColorT c_2 = lab_piece_2.GetColor();
  EXPECT_EQ(c_1, ColorT(0.5, 0.5, 0.5));
  EXPECT_EQ(c_2, ColorT(0.6, 0.6, 0.6));
  EXPECT_NEAR(lab_piece_1.GetEuclideanDistance(lab_piece_2), 9.80226, 0.00001);
}