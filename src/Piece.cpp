//
// Created by Artem Khatchatourov on 14.09.20.
//

#include "Piece.h"

Piece::Piece() {
  Init(0.0, 0.0, 0.0);
}

Piece::Piece(float gray) {
  Init(gray, gray, gray);
}

Piece::Piece(float b, float g, float r) {
  Init(b, g, r);
}

void Piece::Init(float b, float g, float r) {
  color_ = ColorT(b, g, r);
}

double Piece::GetEuclideanDistance(const Piece &other) const {
  return GetEuclideanDistance(other.GetInternalColor());
}

double Piece::GetEuclideanDistance(const ColorT &other_color) const {
  assert(GetInternalColor().channels == other_color.channels);
  double sq_diff = 0.0;
  for (int i = 0; i < GetInternalColor().channels; i++) {
    sq_diff += pow(GetInternalColor()[i] - other_color[i], 2);
  }
  return sq_diff / GetInternalColor().channels;
}