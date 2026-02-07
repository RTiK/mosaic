#include <gtest/gtest.h>
#include <Mosaic/piece/LabIconPiece.hpp>
#include <opencv2/core.hpp>


TEST(LabDistanceTests, DistanceTestSameColors) {
  // Two images with same color distribution but different pixel order
  // Image 1: varying shades of blue
  cv::Vec4b data_1[] = {
      cv::Vec4b(UCHAR_MAX, 0, 0, UCHAR_MAX),
      cv::Vec4b((UCHAR_MAX+1)/2, 0, 0, UCHAR_MAX),
      cv::Vec4b((UCHAR_MAX+1)/4, 0, 0, UCHAR_MAX),
      cv::Vec4b(0, 0, 0, UCHAR_MAX),
  };

  // Image 2: same colors, different order
  cv::Vec4b data_2[] = {
      cv::Vec4b(0, 0, 0, UCHAR_MAX),
      cv::Vec4b((UCHAR_MAX+1)/4, 0, 0, UCHAR_MAX),
      cv::Vec4b((UCHAR_MAX+1)/2, 0, 0, UCHAR_MAX),
      cv::Vec4b(UCHAR_MAX, 0, 0, UCHAR_MAX),
  };

  cv::Mat image_1(2, 2, CV_8UC4, &data_1);
  cv::Mat image_2(2, 2, CV_8UC4, &data_2);

  LabIconPiece piece_1(image_1);
  LabIconPiece piece_2(image_2);

  double distance = LabIconPiece::EuclideanDistance(&piece_1, &piece_2);

  // With k-means, pixel order can affect clustering slightly
  // Lab distances are larger than BGR (L: 0-100, a*/b*: -128..127)
  EXPECT_LT(distance, 60.0) << "Images with same colors should have small distance";
}


TEST(LabDistanceTests, DistanceTestG) {
  // Image 1: 75% black, 25% green
  cv::Vec4b data_1[] = {
      cv::Vec4b(0, 0, 0, UCHAR_MAX),
      cv::Vec4b(0, 0, 0, UCHAR_MAX),
      cv::Vec4b(0, 0, 0, UCHAR_MAX),
      cv::Vec4b(0, UCHAR_MAX, 0, UCHAR_MAX),
  };

  // Image 2: 75% medium-green, 25% light-green
  cv::Vec4b data_2[] = {
      cv::Vec4b(0, UCHAR_MAX/2, 0, UCHAR_MAX),
      cv::Vec4b(0, UCHAR_MAX/2, 0, UCHAR_MAX),
      cv::Vec4b(0, UCHAR_MAX/2, 0, UCHAR_MAX),
      cv::Vec4b(0, UCHAR_MAX/4, 0, UCHAR_MAX),
  };

  cv::Mat image_1(2, 2, CV_8UC4, &data_1);
  cv::Mat image_2(2, 2, CV_8UC4, &data_2);

  LabIconPiece piece_1(image_1);
  LabIconPiece piece_2(image_2);

  double distance = LabIconPiece::EuclideanDistance(&piece_1, &piece_2);

  // Just verify it's a reasonable positive value
  EXPECT_GT(distance, 0.0);
}


TEST(LabDistanceTests, DistanceTestR) {
  // Image 1: All black pixels
  cv::Vec4b data_1[] = {
      cv::Vec4b(0, 0, 0, UCHAR_MAX),
      cv::Vec4b(0, 0, 0, UCHAR_MAX),
      cv::Vec4b(0, 0, 0, UCHAR_MAX),
      cv::Vec4b(0, 0, 0, UCHAR_MAX),
  };

  // Image 2: All red pixels
  cv::Vec4b data_2[] = {
      cv::Vec4b(0, 0, UCHAR_MAX, UCHAR_MAX),
      cv::Vec4b(0, 0, UCHAR_MAX, UCHAR_MAX),
      cv::Vec4b(0, 0, UCHAR_MAX, UCHAR_MAX),
      cv::Vec4b(0, 0, UCHAR_MAX, UCHAR_MAX),
  };

  cv::Mat image_1(2, 2, CV_8UC4, &data_1);
  cv::Mat image_2(2, 2, CV_8UC4, &data_2);

  LabIconPiece piece_1(image_1);
  LabIconPiece piece_2(image_2);

  double distance = LabIconPiece::EuclideanDistance(&piece_1, &piece_2);

  // Black vs Red should have significant Lab distance
  EXPECT_GT(distance, 10.0);
}

TEST(LabDominatingColorTest, DominatingColorTest) {
  // Image with mixed blue values: 2 pixels at full blue, 1 at half blue, 1 at black
  // K-means with 2 clusters should separate these into roughly 2 groups
  cv::Vec4b data[] = {
      cv::Vec4b(UCHAR_MAX, 0, 0, UCHAR_MAX),        // Full blue
      cv::Vec4b((UCHAR_MAX+1)/2, 0, 0, UCHAR_MAX),  // Half blue
      cv::Vec4b(UCHAR_MAX, 0, 0, UCHAR_MAX),        // Full blue
      cv::Vec4b(0, 0, 0, UCHAR_MAX),                // Black
  };

  cv::Mat image(2, 2, CV_8UC4, &data);
  LabIconPiece piece(image);
  cv::Vec3f dominating_color = piece.GetMainColor();

  // GetMainColor now returns Lab values
  // L channel should be > 0 (not completely black since most pixels are blue)
  EXPECT_GT(dominating_color[0], 5.0f) << "L channel should be non-trivial";

  auto colors = piece.GetQuantifiedColors();
  ASSERT_EQ(2, colors.size());
}
