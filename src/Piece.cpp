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
  assert(color_.channels == other.color_.channels);
  double sq_diff = 0.0;
  for (int i = 0; i < color_.channels; i++) {
    sq_diff += pow(color_[i] - other.color_[i], 2);
  }
  return sqrt(sq_diff);
}
