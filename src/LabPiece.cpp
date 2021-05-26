//
// Created by Artem Khatchatourov on 02.03.21.
//

#include "LabPiece.h"


LabPiece::LabPiece(double b, double g, double r) {
  color_bgr_ = ColorT(b, g, r);
  color_ = LabToRgb(color_bgr_);
}

ColorT LabPiece::LabToRgb(const ColorT &BGR) {
  // cvtColor supports only 32F matrices, therefore, assignment is a little more complicated
  // that's one more reason to either move ColorT from double to float or implement custom color conversion in the future
  float bgr[] {(float) BGR[0], (float) BGR[1], (float) BGR[2]};
  cv::Mat mat_in = cv::Mat(cv::Size(1, 1), CV_32FC3);
  mat_in.at<float>(0, 0) = bgr[0];
  mat_in.at<float>(0, 1) = bgr[1];
  mat_in.at<float>(0, 2) = bgr[2];
  cv::Mat mat_out = cv::Mat(cv::Size(1, 1), CV_32FC3);
  cv::cvtColor(mat_in, mat_out, cv::COLOR_BGR2Lab);
  float lab[] {mat_out.at<float>(0, 0), mat_out.at<float>(0, 1), mat_out.at<float>(0, 2)};
  return ColorT(lab[0], lab[1], lab[2]);
}
