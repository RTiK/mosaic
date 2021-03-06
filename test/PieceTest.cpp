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
  EXPECT_NEAR(distance, 0.03, 0.00001);
}

TEST(PieceTests, GrayAssignmentTest) {
  auto piece = Piece(0.5);
  ColorT gray = piece.GetRepresentationColor();
  EXPECT_EQ(gray[0], 0.5);
  EXPECT_EQ(gray[1], 0.5);
  EXPECT_EQ(gray[2], 0.5);
}

/**
 * Tests that LabPiece stores and returns BGR values via getter methods but internally the distance is evaluated
 * using CIELAB.
 */
TEST(PieceTests, LabAssignmentTest) {
  LabPiece lab_piece = LabPiece(0.5, 0.5, 0.5);
  EXPECT_EQ(lab_piece.GetRepresentationColor(), ColorT(0.5, 0.5, 0.5));
  EXPECT_NEAR(lab_piece.GetEuclideanDistance(ColorT(53.4, 0, 0)), 0.0, 0.0001);
}

/**
 * Tests the getters of the internal and external colors.
 */
TEST(PieceTests, LabColorTest) {
  LabPiece lab_piece_B = LabPiece(1.0, 0.0, 0.0);
  LabPiece lab_piece_G = LabPiece(0.0, 1.0, 0.0);
  LabPiece lab_piece_R = LabPiece(0.0, 0.0, 1.0);

  ColorT B = lab_piece_B.GetRepresentationColor();
  ColorT G = lab_piece_G.GetRepresentationColor();
  ColorT R = lab_piece_R.GetRepresentationColor();

  EXPECT_EQ(B, ColorT(1.0, 0.0, 0.0));
  EXPECT_EQ(G, ColorT(0.0, 1.0, 0.0));
  EXPECT_EQ(R, ColorT(0.0, 0.0, 1.0));

  ColorT lab_B = lab_piece_B.GetInternalColor();
  ColorT lab_G = lab_piece_G.GetInternalColor();
  ColorT lab_R = lab_piece_R.GetInternalColor();

  ColorT expect_lab_B = ColorT(32.2937, 79.1875, -107.859);
  ColorT expect_lab_G = ColorT(87.738, -86.1875, 83.1719);
  ColorT expect_lab_R = ColorT(53.241, 80.0938, 67.2031);

  EXPECT_NEAR(lab_B[0], expect_lab_B[0], 0.0001);
  EXPECT_NEAR(lab_B[1], expect_lab_B[1], 0.0001);
  EXPECT_NEAR(lab_B[1], expect_lab_B[1], 0.0001);

  EXPECT_NEAR(lab_G[0], expect_lab_G[0], 0.0001);
  EXPECT_NEAR(lab_G[1], expect_lab_G[1], 0.0001);
  EXPECT_NEAR(lab_G[2], expect_lab_G[2], 0.0001);

  EXPECT_NEAR(lab_R[0], expect_lab_R[0], 0.0001);
  EXPECT_NEAR(lab_R[1], expect_lab_R[1], 0.0001);
  EXPECT_NEAR(lab_R[2], expect_lab_R[2], 0.0001);
}