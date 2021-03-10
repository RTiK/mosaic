//
// Created by Artem Khatchatourov on 14.09.20.
//

#ifndef MOSAIC_PIECE_H
#define MOSAIC_PIECE_H

#include <opencv2/opencv.hpp>

typedef cv::Vec<float, 3> ColorT;

enum Neighbours : unsigned int {
  N = 0x1 << 7,
  NE = 0x1 << 6,
  E = 0x1 << 5,
  SE = 0x1 << 4,
  S = 0x1 << 3,
  SW = 0x1 << 2,
  W = 0x1 << 1,
  NW = 0x1 << 0,
  ALL = N | NE | E | SE | S | SW | W | NW,
  NONE = 0x0
};

class Piece {
  double total_distance_ = 0;
  unsigned int neighbours_ = 0;
  void Init(double b, double g, double r);

 protected:
  ColorT color_;
 public:
  Piece();

  explicit Piece(double gray);

  Piece(double b, double g, double r);

  virtual double GetEuclideanDistance(Piece &other);

  void SetNeighbors(unsigned int neighbours);

  unsigned int GetNeighbors();

  void SetTotalDistance(double total_distance);

  double GetTotalDistance();

  double GetNormalizedDistance() const;

  virtual ColorT GetColor() const { return color_; };
};

#endif //MOSAIC_PIECE_H
