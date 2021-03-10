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

double Piece::GetNormalizedDistance() const {
  unsigned int num_of_neighbors = neighbours_ > 0 ? std::bitset<8>(neighbours_).count() : 1;
  return total_distance_ / num_of_neighbors;
}

void Piece::SetNeighbors(unsigned int neighbours) {
  neighbours_ = neighbours;
}

unsigned int Piece::GetNeighbors() {
  return neighbours_;
}

void Piece::SetTotalDistance(double total_distance) {
  total_distance_ = total_distance;
}

double Piece::GetTotalDistance() {
  return total_distance_;
}

double Piece::GetEuclideanDistance(Piece &other) {
  assert(color_.channels == other.color_.channels);
  double sq_diff = 0.0;
  for (int i = 0; i < color_.channels; i++) {
    sq_diff += pow(color_[i] - other.color_[i], 2);
  }
  return sqrt(sq_diff);
}
