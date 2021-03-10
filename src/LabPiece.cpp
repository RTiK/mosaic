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

/*
 * Converts a color from RGB color model into CIELAB color model via CIEXYZ.
 *
 * Equations taken from
 * Szeliski, Richard. (2011). Computer Vision: Algorithms and Applications. 10.1007/978-1-84882-935-0.

ColorT LabPiece::LabToRgb(const ColorT &RGB) {
  double R = RGB[0], G = RGB[1], B = RGB[2];

  double X = (0.49 * R + 0.31 * G + 0.20 * B);// / 0.17697;
  double Y = (0.17697 * R + 0.81240 * G + 0.01063 * B);// / 0.17697;
  double Z = (0.00 * R + 0.01 * G + 0.99 * B); // / 0.17697;

  std::cout << "X=" << X << " Y=" << Y << " X=" << X << std::endl;
  // Standard Illuminant D65, from wikipedia
  double Xn = 95.0489, Yn = 100.0, Zn = 108.8840;

  auto f = [](double t){
    double delta = 6.0/29.0;
    if (t > pow(delta, 3)) {
      std::cout << "above delta" << std::endl;
      return cbrt(t);
    } else {
      std::cout << "below delta" << std::endl;
      return t / (3.0 * pow(delta, 2)) + 2.0 * delta / 3.0;
    }
  };

  double L = 116.0 * f(Y/Yn) - 16;  // -16 is from wikipedia
  double a = 500.0 * (f(X/Xn) - f(Y/Yn));
  double b = 200.0 * (f(Y/Yn) - f(Z/Zn));

  return ColorT(L, a, b);
}
 */