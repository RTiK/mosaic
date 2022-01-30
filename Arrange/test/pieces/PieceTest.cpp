//
// Created by Artem Khatchatourov on 17.09.20.
//

#include <gtest/gtest.h>
#include "pieces/Piece.h"
#include "pieces/LabPiece.h"
#include "pieces/LabIconClusteringPiece.h"


TEST(PieceTests, DistanceTest) {
  auto p_1 = ColorPiece(0.2, 0.1, 0.1);
  auto p_2 = ColorPiece(0.0, 0.3, 0.2);
  auto distance = p_1.Distance(p_2);
  EXPECT_NEAR(distance, 0.3, 0.00001);
}

TEST(PieceTests, GrayAssignmentTest) {
  auto piece = ColorPiece(0.5);
  cv::Vec3f gray = piece.DominatingColor();
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
  cv::Vec3f lab_color = lab_piece.DominatingColor();
  EXPECT_NEAR(lab_color[0], 53.3875, 0.0001);
  // input color is a shade of gray, thus, the color channels must evaluate to _exactly_ zero
  EXPECT_EQ(lab_color[1], 0.0);
  EXPECT_EQ(lab_color[2], 0.0);
}

/**
 * Tests the getters of the internal and external colors.
 */
TEST(PieceTests, LabColorTest) {
  LabPiece lab_piece_B = LabPiece(1.0, 0.0, 0.0);
  LabPiece lab_piece_G = LabPiece(0.0, 1.0, 0.0);
  LabPiece lab_piece_R = LabPiece(0.0, 0.0, 1.0);

  cv::Vec3f lab_B = lab_piece_B.DominatingColor();
  cv::Vec3f lab_G = lab_piece_G.DominatingColor();
  cv::Vec3f lab_R = lab_piece_R.DominatingColor();

  cv::Vec3f expect_lab_B = cv::Vec3f(32.2937, 79.1875, -107.859);
  cv::Vec3f expect_lab_G = cv::Vec3f(87.738, -86.1875, 83.1719);
  cv::Vec3f expect_lab_R = cv::Vec3f(53.241, 80.0938, 67.2031);

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

TEST(LabIconClusteringPieceTests, DistanceTest) {
  cv::Vec4w data_1[] = {
      cv::Vec4w(USHRT_MAX, 0, 0, USHRT_MAX),  // Lab: 32.3026, 79.1967, -107.8637
      cv::Vec4w(USHRT_MAX, 0, 0, USHRT_MAX),
      cv::Vec4w(0, 0, USHRT_MAX, USHRT_MAX),  // Lab: 53.2329, 80.1093, 67.2201
      cv::Vec4w(0, USHRT_MAX, 0, USHRT_MAX),  // Lab: 87.7370, -86.1846, 83.1812
  };

  cv::Vec4w data_2[] = {
      cv::Vec4w(0, 0, USHRT_MAX, USHRT_MAX),  // Lab: 53.2329, 80.1093, 67.2201
      cv::Vec4w(0, USHRT_MAX, 0, USHRT_MAX),  // Lab: 87.7370, -86.1846, 83.1812
      cv::Vec4w(0, USHRT_MAX, 0, USHRT_MAX),
      cv::Vec4w(0, USHRT_MAX, 0, USHRT_MAX),
  };

  cv::Mat icon_1(2, 2, CV_16UC4, &data_1);
  cv::Mat icon_2(2, 2, CV_16UC4, &data_2);

  LabIconClusteringPiece piece_1(icon_1);
  LabIconClusteringPiece piece_2(icon_2);

  double distance = LabIconClusteringPiece::EuclideanDistance(&piece_1, &piece_2);
  std::cout << distance << std::endl;
}