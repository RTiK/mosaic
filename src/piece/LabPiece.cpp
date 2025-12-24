#include "Mosaic/piece/LabPiece.hpp"


LabPiece::LabPiece(float b, float g, float r) : ColorPiece(b, g, r) {
  lab_color_ = LabToRgb(original_color_);  // L [0..100], a* [-128..127], b* [-128..127]
}

cv::Vec3f LabPiece::LabToRgb(const cv::Vec3f &BGR) {
  auto mat_in = cv::Mat(cv::Size(1, 1), CV_32FC3);
  mat_in.at<cv::Vec3f>(0, 0) = BGR;
  auto mat_out = cv::Mat(cv::Size(1, 1), CV_32FC3);
  cv::cvtColor(mat_in, mat_out, cv::COLOR_BGR2Lab);
  auto lab_color = mat_out.at<cv::Vec3f>(0, 0);
  return cv::Vec3f(lab_color[0], lab_color[1], lab_color[2]);
}

cv::Mat LabPiece::Image(int width, int height) const {
  return cv::Mat(width, height, CV_32FC3, original_color_);
}

double LabPiece::Distance(const Piece &other) const {
  auto o = (LabPiece*) &other;
  return EuclideanDistance(lab_color_, o->lab_color_);
}

std::vector<DominantColor> LabPiece::GetDominantColors() const {
  // LabPiece has only one color with full weight
  return {{lab_color_, 1.0f}};
}
