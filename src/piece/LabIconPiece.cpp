#include "Mosaic/piece/LabIconPiece.hpp"


LabIconPiece::LabIconPiece(cv::Mat image) : IconPiece(image) {
  cv::Mat color, mask;
  SplitColorChannelsAndAlpha(original_image_, color, mask);
  Analyze(color, mask);
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

  lab_ = lab;
  mask_ = mask;

  L_histogram_ = GetHistogram(lab, 0, mask, L_bins_, L_range_);
  a_histogram_ = GetHistogram(lab, 1, mask, a_bins_, a_range_);
  b_histogram_ = GetHistogram(lab, 2, mask, b_bins_, b_range_);
}

double LabIconPiece::Distance(const Piece &other) const {
  return EuclideanDistance(this, (LabIconPiece*) &other);
}

double LabIconPiece::EuclideanDistance(const LabIconPiece *p_1, const LabIconPiece *p_2) {
  cv::Mat L_diff, a_diff, b_diff, L_diff_sq, a_diff_sq, b_diff_sq;
  cv::subtract(p_1->L_histogram_, p_2->L_histogram_, L_diff);
  cv::subtract(p_1->a_histogram_, p_2->a_histogram_, a_diff);
  cv::subtract(p_1->b_histogram_, p_2->b_histogram_, b_diff);
  cv::pow(L_diff, 2, L_diff_sq);
  cv::pow(a_diff, 2, a_diff_sq);
  cv::pow(b_diff, 2, b_diff_sq);

  cv::Scalar_<double> total_distances = cv::sum(L_diff_sq) + cv::sum(a_diff_sq) + cv::sum(b_diff_sq);
  return sqrt(total_distances[0]);
}

// TODO move this method into Analysis so it will be ran only once
cv::Vec3f LabIconPiece::DominatingColor() const {

  double min_val, max_val;
  int min_idx, max_idx;

  float L_step = (L_range_[1] - L_range_[0]) / L_bins_;
  float a_step = (a_range_[1] - a_range_[0]) / a_bins_;
  float b_step = (b_range_[1] - b_range_[0]) / b_bins_;

  // lowest value in the range + index of the max bin * bin size (step) + half a bin to hit the middle of it
  cv::minMaxIdx(L_histogram_, &min_val, &max_val, &min_idx, &max_idx);
  float L_bin = max_idx * L_step + L_step * 0.5;

  cv::minMaxIdx(a_histogram_, &min_val, &max_val, &min_idx, &max_idx);
  float a_bin = a_range_[0] + max_idx * a_step + a_step * 0.5;

  cv::minMaxIdx(b_histogram_, &min_val, &max_val, &min_idx, &max_idx);
  float b_bin = b_range_[0] + max_idx * b_step + b_step * 0.5;

  return cv::Vec3f(L_bin, a_bin, b_bin);
}