//
// Created by Artem Khatchatourov on 14.09.20.
//

#include "Piece.h"

Piece::Piece() {
  Init(0.0, 0.0, 0.0);
}

Piece::Piece(double gray) {
  Init(gray, gray, gray);
}

Piece::Piece(double b, double g, double r) {
  Init(b, g, r);
}

void Piece::Init(double b, double g, double r) {
  color_ = ColorT(b, g, r);
}

double Piece::GetEuclideanDistance(Piece &other) {
  return cv::norm(color_, other.color_, cv::NORM_L2);

}

double Piece::GetEuclideanDistance(ColorT &other_color) {
  return cv::norm(color_, other_color, cv::NORM_L2);
}