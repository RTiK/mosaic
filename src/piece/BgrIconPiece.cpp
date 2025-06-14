#include "Mosaic/piece/BgrIconPiece.hpp"


BgrIconPiece::BgrIconPiece(cv::Mat image) : IconPiece(image)  {
  cv::Mat bgr, mask;
  SplitColorChannelsAndAlpha(original_image_, bgr, mask);
  Analyze(bgr, mask);
}

BgrIconPiece::BgrIconPiece(std::string path) : IconPiece(path) {
  cv::Mat bgr, mask;
  SplitColorChannelsAndAlpha(original_image_, bgr, mask);
  Analyze(bgr, mask);
}

void BgrIconPiece::Analyze(cv::Mat &colors, cv::Mat &mask) {
  assert(colors.type() == CV_32FC3);
  assert(mask.type() == CV_8UC1);

  float range[] = {0, 1};
  b_histogram_ = GetHistogram(colors, 0, mask, b_bins_, range);
  g_histogram_ = GetHistogram(colors, 1, mask, g_bins_, range);
  r_histogram_ = GetHistogram(colors, 2, mask, r_bins_, range);
}

double BgrIconPiece::Distance(const Piece &other) const {
  return EuclideanDistance(this, (BgrIconPiece *) &other);
}

double BgrIconPiece::EuclideanDistance(const BgrIconPiece* p_1, const BgrIconPiece* p_2) {
  cv::Mat b_diff, g_diff, r_diff, b_diff_sq, g_diff_sq, r_diff_sq;
  cv::subtract(p_1->b_histogram_, p_2->b_histogram_, b_diff);
  cv::subtract(p_1->g_histogram_, p_2->g_histogram_, g_diff);
  cv::subtract(p_1->r_histogram_, p_2->r_histogram_, r_diff);
  cv::pow(b_diff, 2, b_diff_sq);
  cv::pow(g_diff, 2, g_diff_sq);
  cv::pow(r_diff, 2, r_diff_sq);

  cv::Scalar_<double> total_distances = cv::sum(b_diff_sq) + cv::sum(g_diff_sq) + cv::sum(r_diff_sq);
  return total_distances[0];
}

cv::Vec3f BgrIconPiece::DominatingColor() const {
  float step = 1.0f / b_bins_;
  double min_val, max_val;
  int min_idx, max_idx;

  cv::minMaxIdx(b_histogram_, &min_val, &max_val, &min_idx, &max_idx);
  float b_bin = max_idx * step + step / 2.0f;

  cv::minMaxIdx(g_histogram_, &min_val, &max_val, &min_idx, &max_idx);
  float g_bin = max_idx * step + step / 2.0f;

  cv::minMaxIdx(r_histogram_, &min_val, &max_val, &min_idx, &max_idx);
  float r_bin = max_idx * step + step / 2.0f;

  return cv::Vec3f(b_bin, g_bin, r_bin);
}