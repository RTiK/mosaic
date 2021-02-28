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

Piece::Piece(double r, double g, double b) {
  Init(r, g, b);
}

void Piece::Init(double r, double g, double b) {
  color_ = ColorT(r, g, b);
}

double GetDistance(Piece &piece_1, Piece &piece_2) {
  return sqrt(pow(piece_1.color_[0] - piece_2.color_[0], 2) + pow(piece_1.color_[1] - piece_2.color_[1], 2)
                  + pow(piece_1.color_[2] - piece_2.color_[2], 2));
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

ColorT Piece::GetColor() {
  return ColorT(color_);
}
