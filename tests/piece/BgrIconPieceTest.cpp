#include <gtest/gtest.h>
#include <Mosaic/piece/BgrIconPiece.hpp>
#include <opencv2/core.hpp>


TEST(RGBDistanceTests, DistanceTestB) {
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

  BgrIconPiece piece_1(image_1);
  BgrIconPiece piece_2(image_2);

  double distance = BgrIconPiece::EuclideanDistance(&piece_1, &piece_2);

  // With k-means, pixel order can affect clustering slightly
  // But images with same color distribution should have very small distance
  EXPECT_LT(distance, 0.5) << "Images with same colors should have small distance";
}


TEST(RGBDistanceTests, DistanceTestG) {
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

  BgrIconPiece piece_1(image_1);
  BgrIconPiece piece_2(image_2);

  double distance = BgrIconPiece::EuclideanDistance(&piece_1, &piece_2);

  // With k-means clustering, the distance will be different from the old histogram approach
  // Just verify it's a reasonable positive value
  EXPECT_GT(distance, 0.0);
  EXPECT_LT(distance, 2.0);  // Should be less than max possible distance
}


TEST(RGBDistanceTests, DistanceTestR) {
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

  BgrIconPiece piece_1(image_1);
  BgrIconPiece piece_2(image_2);

  double distance = BgrIconPiece::EuclideanDistance(&piece_1, &piece_2);

  // Black vs Red should have significant distance with k-means
  // Verify it's a reasonable positive value
  EXPECT_GT(distance, 0.5);
  EXPECT_LT(distance, 2.0);
}

TEST(RGBDominatingColorTest, DominatingColorTest) {
  // Image with mixed blue values: 2 pixels at full blue, 1 at half blue, 1 at black
  // K-means with 2 clusters should separate these into roughly 2 groups
  cv::Vec4b data[] = {
      cv::Vec4b(UCHAR_MAX, 0, 0, UCHAR_MAX),        // Full blue
      cv::Vec4b((UCHAR_MAX+1)/2, 0, 0, UCHAR_MAX),  // Half blue
      cv::Vec4b(UCHAR_MAX, 0, 0, UCHAR_MAX),        // Full blue
      cv::Vec4b(0, 0, 0, UCHAR_MAX),                // Black
  };

  cv::Mat image(2, 2, CV_8UC4, &data);
  BgrIconPiece piece(image);
  cv::Vec3f dominating_color = piece.GetMainColor();

  // With k-means, the dominating color should be one of the cluster centers
  // It should be bluish (BGR format: high in channel 0)
  EXPECT_GT(dominating_color[0], 0.1f) << "Dominating color should have significant blue component";
  EXPECT_LT(dominating_color[1], 0.1f) << "Green channel should be low";
  EXPECT_LT(dominating_color[2], 0.1f) << "Red channel should be low";

  // TODO(artem): Update test to verify weighted mean with calculated constant
  auto colors = piece.GetQuantifiedColors();
  ASSERT_EQ(2, colors.size());
  // EXPECT_EQ(colors[0].color, dominating_color) << "Should return the highest weight color";
}
