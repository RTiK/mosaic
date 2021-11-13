//
// Created by Artem Khatchatourov on 08.11.21.
//

#include "ColorPiece.h"


ColorPiece::ColorPiece() {
  Init(0.0, 0.0, 0.0);
}

ColorPiece::ColorPiece(float gray) {
  Init(gray, gray, gray);
}

ColorPiece::ColorPiece(float b, float g, float r) {
  Init(b, g, r);
}

void ColorPiece::Init(float b, float g, float r) {
  original_color_ = cv::Vec3f(b, g, r);
}

cv::Mat ColorPiece::Image(int width, int height) const {
  return cv::Mat(width, height, CV_32FC3, original_color_);
}

std::string ColorPiece::Print() const {
  return "(" + std::to_string(original_color_[0]) + " " +
               std::to_string(original_color_[1]) + " " +
               std::to_string(original_color_[2]) + ")";
}

double ColorPiece::EuclideanDistance(const cv::Vec3f &c_1, const cv::Vec3f &c_2) {
  assert(c_1.channels == c_2.channels && c_1.channels > 0);
  double sq_diff = 0.0;
  for (int i = 0; i < c_1.channels; i++) {
    sq_diff += pow(c_1[i] - c_2[i], 2);
  }
  return sq_diff / c_1.channels;
}

double ColorPiece::Distance(const Piece &other) const {
  auto* other_color_piece = (ColorPiece*) &other;
  return ColorPiece::EuclideanDistance(original_color_, other_color_piece->original_color_);
}


