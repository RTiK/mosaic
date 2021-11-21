//
// Created by Artem Khatchatourov on 13.11.21.
//

#include "LabIconPiece.h"


LabIconPiece::LabIconPiece(cv::Mat image) : IconPiece(image) {
  cv::Mat lab, mask;
  SplitColorChannelsAndAlpha(original_image_, lab, mask);
  Analyze(lab, mask);
}

LabIconPiece::LabIconPiece(std::string path) : IconPiece(path) {
  cv::Mat color, mask;
  SplitColorChannelsAndAlpha(original_image_, color, mask);
  Analyze(color, mask);
}

void LabIconPiece::Analyze(cv::Mat &color, cv::Mat &mask) {
  assert(color.type() == CV_32FC3);
  assert(mask.type() == CV_8UC1);

  cv::Mat lab;
  cv::cvtColor(color, lab, cv::COLOR_BGR2Lab);

  float L_range[] = {0.0f, 100.0f};
  float a_range[] = {-128.0f, 127.0f};
  float b_range[] = {-128.0f, 127.0f};
  int L_bins = 20;
  int a_bins = 32;
  int b_bins = 32;

  L_histogram_ = GetHistogram(lab, 0, mask, L_bins, L_range);
  a_histogram_ = GetHistogram(lab, 1, mask, a_bins, a_range);
  b_histogram_ = GetHistogram(lab, 2, mask, b_bins, b_range);
}

double LabIconPiece::Distance(const Piece &other) const {
  return EuclideanDistance(this, (LabIconPiece*) &other);
}

double EuclideanDistance(const LabIconPiece *p_1, const LabIconPiece *p_2) {
  cv::Mat L_diff, a_diff, b_diff, L_diff_sq, a_diff_sq, b_diff_sq;
  cv::subtract(p_1->L_histogram_, p_1->L_histogram_, L_diff);
  cv::subtract(p_1->a_histogram_, p_1->a_histogram_, a_diff);
  cv::subtract(p_1->b_histogram_, p_1->b_histogram_, b_diff);
  cv::pow(L_diff, 2, L_diff_sq);
  cv::pow(a_diff, 2, a_diff_sq);
  cv::pow(b_diff, 2, b_diff_sq);

  cv::Scalar_<double> total_distances = cv::sum(L_diff_sq) + cv::sum(a_diff_sq) + cv::sum(b_diff_sq);
  return total_distances[0];
}

cv::Vec3f LabIconPiece::DominatingColor() const {
  double min_val, max_val;
  int min_idx, max_idx;

  cv::minMaxIdx(L_histogram_, &min_val, &max_val, &min_idx, &max_idx);
  float L_bin = max_idx * 5.0 + 2.5f;

  cv::minMaxIdx(a_histogram_, &min_val, &max_val, &min_idx, &max_idx);
  float a_bin = -128 + max_idx * 8 + 4;

  cv::minMaxIdx(b_histogram_, &min_val, &max_val, &min_idx, &max_idx);
  float b_bin = -128 + max_idx * 8 + 4;

  cv::Mat src(1, 1, CV_32FC3, cv::Vec3f(L_bin, a_bin, b_bin));
  cv::Mat dst;
  cv::cvtColor(src, dst, cv::COLOR_Lab2BGR);
  cv::Vec3f bgr = dst.at<cv::Vec3f>(0, 0);
  std::cout << "BGR: " << bgr << std::endl;
  return bgr;
}
