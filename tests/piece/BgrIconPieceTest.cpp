#include <gtest/gtest.h>
#include <Mosaic/piece/BgrIconPiece.hpp>


TEST(RGBDistanceTests, DistanceTestB) {
  cv::Vec4w data_1[] = {
      cv::Vec4w(USHRT_MAX, 0, 0, USHRT_MAX),
      cv::Vec4w((USHRT_MAX+1)/2, 0, 0, USHRT_MAX),
      cv::Vec4w((USHRT_MAX+1)/4, 0, 0, USHRT_MAX),
      cv::Vec4w(0, 0, 0, USHRT_MAX),
  };

  cv::Vec4w data_2[] = {
      cv::Vec4w(0, 0, 0, USHRT_MAX),
      cv::Vec4w((USHRT_MAX+1)/4, 0, 0, USHRT_MAX),
      cv::Vec4w((USHRT_MAX+1)/2, 0, 0, USHRT_MAX),
      cv::Vec4w(USHRT_MAX, 0, 0, USHRT_MAX),
  };

  cv::Mat image_1(2, 2, CV_16UC4, &data_1);
  cv::Mat image_2(2, 2, CV_16UC4, &data_2);

  BgrIconPiece piece_1(image_1);
  BgrIconPiece piece_2(image_2);

  double distance = BgrIconPiece::EuclideanDistance(&piece_1, &piece_2);

  ASSERT_EQ(0, distance);
}


TEST(RGBDistanceTests, DistanceTestG) {
  cv::Vec4w data_1[] = {
      cv::Vec4w(0, 0, 0, USHRT_MAX),
      cv::Vec4w(0, 0, 0, USHRT_MAX),
      cv::Vec4w(0, 0, 0, USHRT_MAX),
      cv::Vec4w(0, USHRT_MAX, 0, USHRT_MAX),
  };

  cv::Vec4w data_2[] = {
      cv::Vec4w(0, USHRT_MAX/2, 0, USHRT_MAX),
      cv::Vec4w(0, USHRT_MAX/2, 0, USHRT_MAX),
      cv::Vec4w(0, USHRT_MAX/2, 0, USHRT_MAX),
      cv::Vec4w(0, USHRT_MAX/4, 0, USHRT_MAX),
  };

  cv::Mat image_1(2, 2, CV_16UC4, &data_1);
  cv::Mat image_2(2, 2, CV_16UC4, &data_2);

  BgrIconPiece piece_1(image_1);
  BgrIconPiece piece_2(image_2);

  double distance = BgrIconPiece::EuclideanDistance(&piece_1, &piece_2);

  ASSERT_EQ(20, distance);
}


TEST(RGBDistanceTests, DistanceTestR) {
  cv::Vec4w data_1[] = {
      cv::Vec4w(0, 0, 0, USHRT_MAX),
      cv::Vec4w(0, 0, 0, USHRT_MAX),
      cv::Vec4w(0, 0, 0, USHRT_MAX),
      cv::Vec4w(0, 0, 0, USHRT_MAX),
  };

  cv::Vec4w data_2[] = {
      cv::Vec4w(0, 0, USHRT_MAX, USHRT_MAX),
      cv::Vec4w(0, 0, USHRT_MAX, USHRT_MAX),
      cv::Vec4w(0, 0, USHRT_MAX, USHRT_MAX),
      cv::Vec4w(0, 0, USHRT_MAX, USHRT_MAX),
  };

  cv::Mat image_1(2, 2, CV_16UC4, &data_1);
  cv::Mat image_2(2, 2, CV_16UC4, &data_2);

  BgrIconPiece piece_1(image_1);
  BgrIconPiece piece_2(image_2);

  double distance = BgrIconPiece::EuclideanDistance(&piece_1, &piece_2);

  ASSERT_EQ(32, distance);
}

TEST(RGBDominatingColorTest, DominatingColorTest) {
  cv::Vec4w data[] = {
      cv::Vec4w(USHRT_MAX, 0, 0, USHRT_MAX),
      cv::Vec4w((USHRT_MAX+1)/2, 0, 0, USHRT_MAX),
      cv::Vec4w(USHRT_MAX, 0, 0, USHRT_MAX),
      cv::Vec4w(0, 0, 0, USHRT_MAX),
  };

  cv::Mat image(2, 2, CV_16UC4, &data);
  BgrIconPiece piece(image);
  cv::Vec3f distance = piece.DominatingColor();

  ASSERT_EQ(cv::Vec3f(USHRT_MAX, 0, 0), distance);
}