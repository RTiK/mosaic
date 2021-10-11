//
// Created by Artem Khatchatourov on 17.10.21.
//

#include "BgrIconPiece.h"


double GetEuclideanDistance(const BgrIconPiece &p_1, const BgrIconPiece &p_2) {
  cv::Mat b_diff, g_diff, r_diff, b_diff_sq, g_diff_sq, r_diff_sq;
  cv::subtract(p_1.b_histogram_, p_2.b_histogram_, b_diff);
  cv::subtract(p_1.g_histogram_, p_2.g_histogram_, g_diff);
  cv::subtract(p_1.r_histogram_, p_2.r_histogram_, r_diff);
  cv::pow(b_diff, 2, b_diff_sq);
  cv::pow(g_diff, 2, g_diff_sq);
  cv::pow(r_diff, 2, r_diff_sq);

  cv::Scalar_<double> total_distances = cv::sum(b_diff_sq) + cv::sum(g_diff_sq) + cv::sum(r_diff_sq);
  return total_distances[0];
}

void BgrIconPiece::Analyze(cv::Mat &colors, cv::Mat &mask) {
  assert(colors.type() == CV_32FC3);
  assert(mask.type() == CV_8UC1);

  float range[] = {0, 1};
  b_histogram_ = GetHistogram(colors, 0, mask, 32, range);
  g_histogram_ = GetHistogram(colors, 1, mask, 32, range);
  r_histogram_ = GetHistogram(colors, 2, mask, 32, range);
}

BgrIconPiece::BgrIconPiece(cv::Mat image) : IconPiece(image)  {
  cv::Mat bgr, mask;
  SplitColorChannelsAndAlpha(original_image_, bgr, mask);
  Analyze(bgr, mask);
}

cv::Mat BgrIconPiece::GetImage(int width, int height) const {
  cv::Mat resized_image;
  cv::resize(original_image_, resized_image, cv::Size(width, height));
  return resized_image;
}

double BgrIconPiece::GetEuclideanDistance(const BgrIconPiece &other) const {
  std::cout << "asdf" << std::endl;
  cv::Mat b_diff, g_diff, r_diff, b_diff_sq, g_diff_sq, r_diff_sq;
  cv::subtract(b_histogram_, other.b_histogram_, b_diff);
  cv::subtract(g_histogram_, other.g_histogram_, g_diff);
  cv::subtract(r_histogram_, other.r_histogram_, r_diff);
  cv::pow(b_diff, 2, b_diff_sq);
  cv::pow(g_diff, 2, g_diff_sq);
  cv::pow(r_diff, 2, r_diff_sq);

  cv::Scalar_<double> total_distances = cv::sum(b_diff_sq) + cv::sum(g_diff_sq) + cv::sum(r_diff_sq);
  return total_distances[0];
}
