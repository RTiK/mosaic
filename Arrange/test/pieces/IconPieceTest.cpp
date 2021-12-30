//
// Created by Artem Khatchatourov on 13.10.21.
//

#include "pieces/IconPiece.h"
#include "pieces/BgrIconPiece.h"
#include "pieces/LabIconPiece.h"
#include "gtest/gtest.h"

TEST(SplitColorChannelsAndAlphaTest, Simple) {
  // we expect the image to be of type CV_16UC4, which houses ushorts
  cv::Vec4w bgra((USHRT_MAX+1)/8, (USHRT_MAX+1)/4, (USHRT_MAX+1)/2, USHRT_MAX);
  cv::Vec4w data[] = {bgra};
  cv::Mat mat(1, 1, CV_16UC4, &data);

  cv::Mat colors, alpha;
  IconPiece::SplitColorChannelsAndAlpha(mat, colors, alpha);

  cv::Vec4f color_data = colors.at<cv::Vec4f>(0, 0);
  ASSERT_NEAR(1.0/8, color_data[0], 0.0001);
  ASSERT_NEAR(1.0/4, color_data[1], 0.0001);
  ASSERT_NEAR(1.0/2, color_data[2], 0.0001);
  ASSERT_EQ(1, *alpha.data);
}


TEST(SplitColorChannelsAndAlphaTest, TestTypes) {
  cv::Vec4w bgra(0, 0, 0, 0);
  cv::Vec4w data[] = {bgra};
  cv::Mat mat(1, 1, CV_16UC4, &data);

  cv::Mat colors, alpha;
  IconPiece::SplitColorChannelsAndAlpha(mat, colors, alpha);

  ASSERT_EQ(CV_32FC3, colors.type());
  ASSERT_EQ(CV_8UC1, alpha.type());
}


TEST(SplitColorChannelsAndAlphaTest, TestAlpha) {
  cv::Vec4w alpha_0(0, 0, 0, 0);
  cv::Vec4w alpha_1(0, 0, 0, 1);
  cv::Vec4w alpha_255(0, 0, 0, 255);
  cv::Vec4w alpha_65535(0, 0, 0, 65535);  // USHRT_MAX

  cv::Vec4w data[2][2] {
      {alpha_0, alpha_1},
      {alpha_255, alpha_65535}
  };
  cv::Mat mat(2, 2, CV_16UC4, &data);

  cv::Mat colors, alpha;
  IconPiece::SplitColorChannelsAndAlpha(mat, colors, alpha);

  ASSERT_EQ(0, alpha.at<uchar>(0, 0));
  ASSERT_EQ(1, alpha.at<uchar>(0, 1));
  ASSERT_EQ(1, alpha.at<uchar>(1, 0));
  ASSERT_EQ(1, alpha.at<uchar>(1, 1));
}


TEST(GetHistogramTests, EvenDistributionTest) {
  float data[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
  cv::Mat colors(1, 11, CV_32FC1, &data);

  float range[] = {0.0, 1.00001};  // the upper limit is excluded
  cv::Mat histogram = IconPiece::GetHistogram(colors, 0, cv::Mat(), 11, range);
  cv::Mat expected = cv::Mat::ones(11, 1, CV_32FC1);

  // compare arrays element-wise, non-equal indices will be set to 255 in the diffs array, 0 otherwise
  cv::Mat diffs;
  cv::compare(expected, histogram, diffs, cv::CMP_NE);
  ASSERT_EQ(0, cv::countNonZero(diffs));
}


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

  double distance = EuclideanDistance(&piece_1, &piece_2);

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

  double distance = EuclideanDistance(&piece_1, &piece_2);

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

  double distance = EuclideanDistance(&piece_1, &piece_2);

  ASSERT_EQ(32, distance);
}

TEST(AssignmentTest, LabIconTests) {
  cv::Vec4w bgra(USHRT_MAX, 0, 0, USHRT_MAX);

  cv::Mat image(2, 2, CV_16UC4, bgra);

  LabIconPiece piece(image);

  EXPECT_NEAR(32.5, piece.DominatingColor()[0], 0.0001);
  EXPECT_NEAR(84.0, piece.DominatingColor()[1], 0.0001);
  EXPECT_NEAR(-108.0, piece.DominatingColor()[2], 0.0001);
}