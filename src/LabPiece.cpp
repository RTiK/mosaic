//
// Created by Artem Khatchatourov on 02.03.21.
//

#include "LabPiece.h"


LabPiece::LabPiece(double b, double g, double r) {
  color_bgr_ = ColorT(b, g, r);
  color_ = LabToRgb(color_bgr_);
}

ColorT LabPiece::LabToRgb(const ColorT &BGR) {
  auto mat_in = cv::Mat(cv::Size(1, 1), CV_32FC3);
  mat_in.at<ColorT>(0, 0) = BGR;
  auto mat_out = cv::Mat(cv::Size(1, 1), CV_32FC3);
  cv::cvtColor(mat_in, mat_out, cv::COLOR_BGR2Lab);
  auto lab_color = mat_out.at<ColorT>(0, 0);
  return ColorT(lab_color[0], lab_color[1], lab_color[2]);
}
